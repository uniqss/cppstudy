// Code generated by little bull tool. DO NOT EDIT!!!

package DataTables

type DT_Currency_Config struct {
	Id          uint32 `db:"id"`            // 序列
	TestInt1    uint32 `db:"test_int_1"`    // 测试int1
	TestInt2    uint32 `db:"test_int_2"`    // 测试int2
	TestString1 string `db:"test_string_1"` // 测试string
	TestString2 string `db:"test_string_2"` // 测试string
}

func (m *DT_Currency_Config) GetId() uint32 {
	if m != nil {
		return m.Id
	}
	return 0
}

func (m *DT_Currency_Config) GetTestInt1() uint32 {
	if m != nil {
		return m.TestInt1
	}
	return 0
}

func (m *DT_Currency_Config) GetTestInt2() uint32 {
	if m != nil {
		return m.TestInt2
	}
	return 0
}

func (m *DT_Currency_Config) GetTestString1() string {
	if m != nil {
		return m.TestString1
	}
	return ""
}

func (m *DT_Currency_Config) GetTestString2() string {
	if m != nil {
		return m.TestString2
	}
	return ""
}

type DT_Currency_Config_Data struct {
	DT_Currency_ConfigItems map[uint32]*DT_Currency_Config
}

func (dt *DT_Currency_Config_Data) MakeMap() {
	dt.DT_Currency_ConfigItems = make(map[uint32]*DT_Currency_Config)
}

func init() {
	register("DT_Currency_Config", &DT_Currency_Config_Data{}, &DT_Currency_Config{})
}

func (dt *DT_Currency_Config) FromData(data []interface{}) {
	dt.Id = DataTableReadUInt32(data, "Id", 0, "DT_Currency_Config")
	dt.TestInt1 = DataTableReadUInt32(data, "TestInt1", 1, "DT_Currency_Config")
	dt.TestInt2 = DataTableReadUInt32(data, "TestInt2", 2, "DT_Currency_Config")
	dt.TestString1 = DataTableReadString(data, "TestString1", 3, "DT_Currency_Config")
	dt.TestString2 = DataTableReadString(data, "TestString2", 4, "DT_Currency_Config")

	GetDT_Currency_Config().DT_Currency_ConfigItems[dt.Id] = dt
}

func GetDT_Currency_Config() *DT_Currency_Config_Data {
	return get("DT_Currency_Config").(*DT_Currency_Config_Data)
}
