let outputDir = "gen_go"

// Check if a file or directory exists in this path
let exists = function(file) {
    let ret = os.rename(file, file);
    if (ret === 0) {
        return true;
    }

    // 13 means: Permission denied, but it exists
    return ret === 13;
}
// Check if a directory exists in this path
let isdir = function(path) {
    // "/" works on both Unix and Windows
    return exists(path + "/");
}

let createDirIfNotExists = function(path) {
    if (isdir(path)) {
        return;
    }
    os.execute("mkdir " + path);
}

let ProcessOneSheet = function(xlsxName, sheetName, vecNames, vecTypes, vecDescriptions) {
    print("fun begin");
    print("xlsxName:", xlsxName, " sheetName:", sheetName, " vecNames:", vecNames, " vecTypes:", vecTypes, " vecDescriptions:", vecDescriptions);

    createDirIfNotExists(outputDir);

    let count1 = vecNames.length;
    let count2 = vecTypes.length;
    let count3 = vecDescriptions.length;
    let count = Math.min(count1, count2, count3);

    print("count1:", count1, " count2:", count2, " count3:", count3, " count:", count);

    let filePath = "./" + outputDir + "/" + sheetName + ".go";
    let fw = new FileWriter(filePath);
    fw.println("hello world!");
    fw.println("xlsxName:", xlsxName, " sheetName:", sheetName);
    fw.println("hello world!");
    print("fun end");
};

ProcessOneSheet