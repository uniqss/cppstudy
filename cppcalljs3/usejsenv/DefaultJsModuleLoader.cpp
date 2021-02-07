
#include "stl.h"
#include "def.h"


static FString PathNormalize(const FString& PathIn)
{
	TArray<FString> PathFrags;
	PathIn.ParseIntoArray(PathFrags, TEXT("/"));
	Algo::Reverse(PathFrags);
	TArray<FString> NewPathFrags;
	while (PathFrags.Num() > 0) {
		FString E = PathFrags.Pop();
		if (E != TEXT("") && E != TEXT("."))
		{
			if (E == TEXT("..") && NewPathFrags.Num() > 0 && NewPathFrags.Last() != TEXT("..")) {
				NewPathFrags.Pop();
			}
			else {
				NewPathFrags.Push(E);
			}
		}
	}
	return FString::Join(NewPathFrags, TEXT("/"));
}

bool DefaultJSModuleLoader::CheckExists(const FString& PathIn, FString& Path, FString& AbsolutePath)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString NormalizedPath = PathNormalize(PathIn);
	if (PlatformFile.FileExists(*NormalizedPath))
	{
		AbsolutePath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*NormalizedPath);
#if PLATFORM_WINDOWS
		auto PathInWin = AbsolutePath.Replace(TEXT("/"), TEXT("\\"));
		if (PathInWin.Len() && PathInWin[1] == ':')
		{
			PathInWin = PathInWin.Mid(0, 1).ToLower() + PathInWin.Mid(1);
		}
#endif

		Path = NormalizedPath;

		return true;
	}

	return false;
}

bool DefaultJSModuleLoader::SearchModuleInDir(const FString& Dir, const FString& RequiredModule, FString& Path, FString& AbsolutePath)
{
	if (FPaths::GetExtension(RequiredModule) == TEXT(""))
	{
		return SearchModuleWithExtInDir(Dir, RequiredModule + ".js", Path, AbsolutePath)
			|| SearchModuleWithExtInDir(Dir, RequiredModule / "index.js", Path, AbsolutePath)
			|| SearchModuleWithExtInDir(Dir, RequiredModule / "package.json", Path, AbsolutePath);
	}
	else
	{
		return SearchModuleWithExtInDir(Dir, RequiredModule, Path, AbsolutePath);
	}
}

bool DefaultJSModuleLoader::SearchModuleWithExtInDir(const FString& Dir, const FString& RequiredModule, FString& Path, FString& AbsolutePath)
{
	return CheckExists(Dir / RequiredModule, Path, AbsolutePath)
		|| (!Dir.EndsWith(TEXT("node_modules")) && CheckExists(Dir / TEXT("node_modules") / RequiredModule, Path, AbsolutePath));
}

bool DefaultJSModuleLoader::Search(const FString& RequiredDir, const FString& RequiredModule, FString& Path, FString& AbsolutePath)
{
	if (SearchModuleInDir(RequiredDir, RequiredModule, Path, AbsolutePath))
	{
		return true;
	}
	else if (RequiredDir != TEXT("") && !RequiredModule.GetCharArray().Contains('/') && !RequiredModule.EndsWith(TEXT(".js")))
	{
		// ����require���ļ����ڵ�Ŀ¼������
		TArray<FString> pathFrags;
		RequiredDir.ParseIntoArray(pathFrags, TEXT("/"));
		pathFrags.Pop(); // has try in "if (SearchModuleInDir(RequiredDir, RequiredModule, Path, AbsolutePath))"
		while (pathFrags.Num() > 0)
		{
			if (!pathFrags.Last().Equals(TEXT("node_modules")))
			{
				if (SearchModuleInDir(FString::Join(pathFrags, TEXT("/")), RequiredModule, Path, AbsolutePath))
				{
					return true;
				}
			}
			pathFrags.Pop();
		}
	}

	return SearchModuleInDir(FPaths::ProjectContentDir() / ScriptRoot, RequiredModule, Path, AbsolutePath)
		|| (ScriptRoot != TEXT("JavaScript") && SearchModuleInDir(FPaths::ProjectContentDir() / TEXT("JavaScript"), RequiredModule, Path, AbsolutePath));
}

bool DefaultJSModuleLoader::Load(const FString& Path, TArray<uint8>& Content)
{
	//return (FPaths::FileExists(FullPath) && FFileHelper::LoadFileToString(Content, *FullPath));
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	IFileHandle* FileHandle = PlatformFile.OpenRead(*Path);
	if (FileHandle) {
		int len = FileHandle->Size();
		Content.Reset(len + 2);
		Content.AddUninitialized(len);
		FileHandle->Read(Content.GetData(), len);
		delete FileHandle;

		return true;
	}
	return false;
}

FString& DefaultJSModuleLoader::GetScriptRoot()
{
	return ScriptRoot;
}

