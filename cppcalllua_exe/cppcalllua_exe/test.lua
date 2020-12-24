local function camelCase(s)
	return string.gsub(s, "_%w+", function(word)
		local first = string.sub(word, 2, 2)
		local rest = string.sub(word, 3)
		return string.upper(first) .. rest
	end)
end
local function CamelCase(s)
    local camel = camelCase(s)
    print("camel:"..camel)
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
    end
end

str = "snake_case_hello_world_123_haha"

camel = CamelCase(str)
print(camel)

--[[
snake = snake_case(camel)
print(snake)
--]]

print(transType("uint32"))
print(transType("string"))
print(transType("repeated string"))
print(transType("repeated uint32"))