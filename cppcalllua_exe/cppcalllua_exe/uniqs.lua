local outputDir = "gen_go"
local goFile = nil
local function PrintTable1(vecDatas)
    print("lua table elements:")
    for tKey, tValue in pairs(vecDatas) do
        print("\t "..tKey.." => "..tValue)
    end
end
local function tablelength(T)
    local count = 0
    for _ in pairs(T) do count = count + 1 end
    return count
end

local function writeln(str)
    if str ~= nil then
        goFile:write(str, "\n")
    else
        goFile:write("\n")
    end
end
local function camelCase(s)
	return string.gsub(s, "_%w+", function(word)
		local first = string.sub(word, 2, 2)
		local rest = string.sub(word, 3)
		return string.upper(first) .. rest
	end)
end
local function CamelCase(s)
    local camel = camelCase(s)
    --print("camel:"..camel)
    return (camel:gsub("^%l", string.upper))
end
local function transType(ctype)
    if ctype == "uint32" then
        return "uint32"
    elseif ctype == "string" then
        return "string"
    elseif ctype == "repeated string" then
        return "[]string"
    elseif ctype == "repeated uint32" then
        return "[]uint32"
    else
        print("type unrecogonized:"..ctype)
    end
end
local function getTypeDefault(ctype)
    if ctype == "uint32" then
        return "0"
    elseif ctype == "string" then
        return "\"\""
    elseif ctype == "repeated string" then
        return "nil"
    elseif ctype == "repeated uint32" then
        return "nil"
    else
        print("type unrecogonized:"..ctype)
    end
end

function ProcessOneSheet(xlsxName, sheetName, vecNames, vecTypes, vecDescriptions)
    --[[
    print("xlsxName:" .. xlsxName)
    print("sheetName:" .. sheetName)
    PrintTable1(vecNames)
    PrintTable1(vecTypes)
    PrintTable1(vecDescriptions)
    --]]

    os.execute("mkdir " .. outputDir)

    -- 取count的最小值
    local count1 = tablelength(vecNames)
    local count2 = tablelength(vecTypes)
    local count3 = tablelength(vecDescriptions)
    local count = math.min(count1, count2)
    count = math.min(count, count3)

    local filePath = "./"..outputDir.."/" ..sheetName..".go"

    goFile = io.open(filePath, "w+")
    if goFile == nil then
        print("filePath:"..filePath.." goFile == nil")
    else
        --print("filePath:"..filePath.." goFile not nil")
    end

    writeln("// Code generated by little bull tool. DO NOT EDIT!!!")
    writeln()
    writeln("package DataTables")
    writeln()
    writeln("type "..sheetName.." struct {")

    for idx=1,count do
        local cname = vecNames[idx]
        local ctype = vecTypes[idx]
        if cname ~= nil and cname ~= "" and ctype ~= nil and ctype ~= "" then
            ctype = transType(ctype)
            local cdesc = vecDescriptions[idx]
            writeln("\t"..CamelCase(cname).." "..ctype.."  `db:\""..cname.."\"` // "..cdesc)
        end
    end
    writeln("}")
    writeln()

    for idx = 1, count do
        --[[
func (m *DT_Hero_Nature_Config) GetQiRate() []uint32 {
	if m != nil {
		return m.QiRate
	}
	return nil
}
        --]]
        local cname = CamelCase(vecNames[idx])
        local ctype = vecTypes[idx]
        if cname ~= nil and cname ~= "" and ctype ~= nil and ctype ~= "" then
            ctype = transType(ctype)
            writeln("func (m *"..sheetName..") Get"..cname.."() "..ctype.." {")
            writeln("\tif m != nil {")
            writeln("\t\treturn m."..cname)
            writeln("\t}")
            writeln("\treturn "..getTypeDefault(vecTypes[idx]))
            writeln("}")
            writeln()
        end
    end
    writeln()

    writeln("type "..sheetName.."_Data struct {")

    writeln("\t"..sheetName.."Items map[uint32]*"..sheetName.."")

    writeln("}")
    writeln()

    writeln("func (dt *"..sheetName.."_Data) MakeMap(){")

    writeln("\tdt."..sheetName.."Items = make(map[uint32]*"..sheetName..")")

    writeln("}")
    writeln()

    writeln("func init() {")
    writeln("\tDTPreInit()")
    writeln("\tregister(\""..sheetName.."\", &"..sheetName.."_Data{}, &"..sheetName.."{},")
    writeln("\t\tfunc(dt interface{}, bt interface{}) {")
    writeln("\t\t\tdt.(*"..sheetName.."_Data)."..sheetName.."Items[bt.(*"..sheetName..").Id] = bt.(*"..sheetName..")")
    writeln("\t\t})")
    writeln("}")
    writeln()

    writeln("func (dt *"..sheetName..")FromData(data []interface{}) {")
    local realIdx = 0
    for idx = 1, count do
        local cname = CamelCase(vecNames[idx])
        local ctype = vecTypes[idx]
        if cname ~= nil and cname ~= "" and ctype ~= nil and ctype ~= "" then
            ctype = transType(ctype)
            if ctype == "uint32" then
                writeln("dt."..cname.." = DataTableReadUInt32(data, \""..cname.."\", "..realIdx..", \""..sheetName.."\")")
            elseif ctype == "string" then
                writeln("dt."..cname.." = DataTableReadString(data, \""..cname.."\", "..realIdx..", \""..sheetName.."\")")
            elseif ctype == "[]uint32" then
                writeln("dt."..cname.." = DataTableReadUInt32Arr(data, \""..cname.."\", "..realIdx..", \""..sheetName.."\")")
            elseif ctype == "[]string" then
                writeln("dt."..cname.." = DataTableReadStringArr(data, \""..cname.."\", "..realIdx..", \""..sheetName.."\")")
            end
            realIdx = realIdx + 1
        end
    end
    writeln("}")
    writeln()

    writeln("func Get"..sheetName.."() *"..sheetName.."_Data {")
    writeln("\treturn get(\""..sheetName.."\").(*"..sheetName.."_Data)")
    writeln("}")
    
    io.close(goFile)
    return 0
end
