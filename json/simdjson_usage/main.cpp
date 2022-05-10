#include <iostream>
#include "simdjson.h"
#include "simdjson.cpp"

using std::cout;
using std::endl;

// bool read_one_line(simdjson::simdjson_result<simdjson::fallback::ondemand::field>& val) {
//     cout << val["int1"] << endl;
//     cout << val["int2"] << endl;
//     cout << val["str2server"] << endl;
//     cout << val["intarray1"] << endl;
//     cout << val["stringarray1"] << endl;
//     cout << val["intarray2"] << endl;
//     cout << val["stringarray2"] << endl;


//     return true;
// }


bool _TrimJsonValueString(simdjson::ondemand::value& root, const char* node_name, std::string& out) {
    auto field_result = root.find_field(node_name);
    if (field_result.error() != simdjson::SUCCESS) return false;
    simdjson::ondemand::value field = field_result.value();
    simdjson::simdjson_result<std::string_view> result = field.get_string();

    if (result.error() != simdjson::SUCCESS) return false;

    out = result.value();
    return true;
}

bool _TrimJsonValueInt(simdjson::ondemand::value& root, const char* node_name, int& out, bool from_string = false) {
    auto field_result = root.find_field(node_name);
    if (field_result.error() != simdjson::SUCCESS) return false;
    simdjson::ondemand::value field = field_result.value();
    simdjson::simdjson_result<int64_t> result;
    if (field.is_integer()) {
        result = field.get_int64();
    } else if (from_string) {
        result = field.get_int64_in_string();
    } else {
        return false;
    }

    if (result.error() != simdjson::SUCCESS) return false;

    out = (int)result.value();
    return true;
}

bool _TrimJsonValueUint(simdjson::ondemand::value& root, const char* node_name, uint& out, bool from_string = false) {
    auto field_result = root.find_field(node_name);
    if (field_result.error() != simdjson::SUCCESS) return false;
    simdjson::ondemand::value field = field_result.value();
    simdjson::simdjson_result<int64_t> result;
    if (field.is_integer()) {
        result = field.get_int64();
    } else if (from_string) {
        result = field.get_int64_in_string();
    } else {
        return false;
    }

    if (result.error() != simdjson::SUCCESS) return false;

    out = (uint)result.value();
    return true;
}

bool _TrimJsonValueInt64(simdjson::ondemand::value& root, const char* node_name, int64_t& out, bool from_string = false) {
    auto field_result = root.find_field(node_name);
    if (field_result.error() != simdjson::SUCCESS) return false;
    simdjson::ondemand::value field = field_result.value();
    simdjson::simdjson_result<int64_t> result;
    if (field.is_integer()) {
        result = field.get_int64();
    } else if (from_string) {
        result = field.get_int64_in_string();
    } else {
        return false;
    }

    if (result.error() != simdjson::SUCCESS) return false;

    out = result.value();
    return true;
}

bool _TrimJsonValueUint64(simdjson::ondemand::value& root, const char* node_name, uint64_t& out, bool from_string = false) {
    auto field_result = root.find_field(node_name);
    if (field_result.error() != simdjson::SUCCESS) return false;
    simdjson::ondemand::value field = field_result.value();
    simdjson::simdjson_result<uint64_t> result;
    if (field.is_integer()) {
        result = field.get_uint64();
    } else if (from_string) {
        result = field.get_uint64_in_string();
    } else {
        return false;
    }

    if (result.error() != simdjson::SUCCESS) return false;

    out = result.value();
    return true;
}

int main(void) {
    int itemp = 0;
    std::string stemp;
    bool ok = false;

    simdjson::ondemand::parser parser;
    auto result_json = simdjson::padded_string::load("test.json");
    if (result_json.error() != simdjson::SUCCESS) return -1;
    simdjson::ondemand::document doc = parser.iterate(result_json);
    auto result_obj = doc.get_object();
    if (result_obj.error() != simdjson::SUCCESS) return -1;
    for (auto it : result_obj) {
        simdjson::ondemand::value itvalue = it.value();

        ok = _TrimJsonValueInt(itvalue, "int1", itemp);
        cout << ok << endl;
        if (ok) cout << itemp << endl;

        ok = _TrimJsonValueInt(itvalue, "int2x", itemp);
        // ok = _TrimJsonValueInt(itvalue, "int2", itemp);
        cout << ok << endl;
        if (ok) cout << itemp << endl;

        ok = _TrimJsonValueString(itvalue, "str2serverx", stemp);
        // ok = _TrimJsonValueString(itvalue, "str2server", stemp);
        cout << ok << endl;
        if (ok) cout << stemp << endl;

        // auto vi = itvalue.find_field("int1").get_int64();
        // simdjson::ondemand::value fuck = itvalue.find_field("int1");
        // cout << fuck.type() << endl;
        // cout << fuck.get_number().error() << endl;
        // cout << fuck.get_int64() << endl;
        cout << "##################################" << endl;
    }
}
