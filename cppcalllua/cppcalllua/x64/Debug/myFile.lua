print("this is printed on calling luaL_dofile")

print("test chinese: 中文乱码")

function add(a, b)
    print("add called. a:"..a.." b:"..b)
    return a + b
end

function testWriteFile()
    sql = "insert into \"asdf\" values('aaa', 'bbb', '中文');"
    file = io.open ("test.sql", "a")
    file:write(sql)
    io.close(file)
    return 0
end
