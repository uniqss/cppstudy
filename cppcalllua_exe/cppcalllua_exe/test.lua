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

str = "snake_case_hello_world_123_haha"

camel = CamelCase(str)
print(camel)

--[[
snake = snake_case(camel)
print(snake)
--]]