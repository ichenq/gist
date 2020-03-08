// Copyright © 2017 ichenq@outlook.com All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.

package strutil

import (
	"fmt"
	"reflect"
)

//struct是否有此字段
func HasField(ptr interface{}, key string) bool {
	object := reflect.ValueOf(ptr).Elem()
	field := object.FieldByName(key)
	return field.IsValid()
}

//获取struct的一个整数字段值
func GetIntField(ptr interface{}, key string) (int64, error) {
	object := reflect.ValueOf(ptr).Elem()
	name := object.Type().Name()
	field := object.FieldByName(key)
	if !field.IsValid() {
		return 0, fmt.Errorf("%s.%s field does not exist ", name, key)
	}
	switch field.Kind() {
	case reflect.Int, reflect.Int8, reflect.Int16, reflect.Int32, reflect.Int64:
		return field.Int(), nil
	case reflect.Uint, reflect.Uint8, reflect.Uint16, reflect.Uint32, reflect.Uint64:
		return int64(field.Uint()), nil
	default:
		return 0, fmt.Errorf("GetIntField: %s.%s type mismatch, expect integer, got [%s]", name, key, field.Type().Name())
	}
}

//设置struct的一个整数字段值
func SetIntField(ptr interface{}, key string, value int64) error {
	object := reflect.ValueOf(ptr).Elem()
	name := object.Type().Name()
	field := object.FieldByName(key)
	if !field.IsValid() {
		return fmt.Errorf("%s.%s field does not exist ", name, key)
	}
	if !field.CanSet() {
		return fmt.Errorf("%s.%s cannot be set", name, key)
	}
	// TO-DO: integer overflow check
	switch field.Kind() {
	case reflect.Int, reflect.Int8, reflect.Int16, reflect.Int32, reflect.Int64:
		field.SetInt(value)
	case reflect.Uint, reflect.Uint8, reflect.Uint16, reflect.Uint32, reflect.Uint64:
		field.SetUint(uint64(value))
	default:
		return fmt.Errorf("SetIntField: %s.%s type mismatch, expect integer, got [%s]", name, key, field.Type().Name())
	}
	return nil
}

//获取struct的一个浮点数字段值
func GetFloatField(ptr interface{}, key string) (float64, error) {
	object := reflect.ValueOf(ptr).Elem()
	name := object.Type().Name()
	field := object.FieldByName(key)
	if !field.IsValid() {
		return 0, fmt.Errorf("%s.%s field does not exist ", name, key)
	}
	switch field.Kind() {
	case reflect.Float32, reflect.Float64:
		return field.Float(), nil
	default:
		return 0, fmt.Errorf("GetFloatField: %s.%s type mismatch, expect float, got [%s]", name, key, field.Type().Name())
	}
}

//设置struct的一个浮点数字段值
func SetFloatField(ptr interface{}, key string, value float64) error {
	object := reflect.ValueOf(ptr).Elem()
	name := object.Type().Name()
	field := object.FieldByName(key)
	if !field.IsValid() {
		return fmt.Errorf("%s.%s field does not exist ", name, key)
	}
	if !field.CanSet() {
		return fmt.Errorf("%s.%s cannot be set", name, key)
	}
	switch field.Kind() {
	case reflect.Float32, reflect.Float64:
		field.SetFloat(value)
	default:
		return fmt.Errorf("SetFloatField: %s.%s type mismatch, expect bool, got [%s]", name, key, field.Type().Name())
	}
	return nil
}

//获取struct的一个布尔字段值
func GetBoolField(ptr interface{}, key string) (bool, error) {
	object := reflect.ValueOf(ptr).Elem()
	name := object.Type().Name()
	field := object.FieldByName(key)
	if !field.IsValid() {
		return false, fmt.Errorf("%s.%s field does not exist ", name, key)
	}
	switch field.Kind() {
	case reflect.Bool:
		return field.Bool(), nil
	default:
		return false, fmt.Errorf("GetBoolField: %s.%s type mismatch, expect bool, got [%s]", name, key, field.Type().Name())
	}
}

//设置struct的一个布尔字段值
func SetBoolField(ptr interface{}, key string, value bool) error {
	object := reflect.ValueOf(ptr).Elem()
	name := object.Type().Name()
	field := object.FieldByName(key)
	if !field.IsValid() {
		return fmt.Errorf("%s.%s field does not exist ", name, key)
	}
	if !field.CanSet() {
		return fmt.Errorf("%s.%s cannot be set", name, key)
	}
	switch field.Kind() {
	case reflect.Bool:
		field.SetBool(value)
	default:
		return fmt.Errorf("SetBoolField: %s.%s type mismatch, expect bool, got [%s]", name, key, field.Type().Name())
	}
	return nil
}

//获取struct的一个字符串字段值
func GetStringField(ptr interface{}, key string) (string, error) {
	object := reflect.ValueOf(ptr).Elem()
	name := object.Type().Name()
	field := object.FieldByName(key)
	if !field.IsValid() {
		return "", fmt.Errorf("%s.%s field does not exist ", name, key)
	}
	switch field.Kind() {
	case reflect.String:
		return field.String(), nil
	default:
		return "", fmt.Errorf("GetStringField: %s.%s type mismatch, expect string, got [%s]", name, key, field.Type().Name())
	}
}

//设置struct的一个字符串字段值
func SetStringField(ptr interface{}, key string, value string) error {
	object := reflect.ValueOf(ptr).Elem()
	name := object.Type().Name()
	field := object.FieldByName(key)
	if !field.IsValid() {
		return fmt.Errorf("%s.%s field does not exist ", name, key)
	}
	if !field.CanSet() {
		return fmt.Errorf("%s.%s cannot be set", name, key)
	}
	switch field.Kind() {
	case reflect.String:
		field.SetString(value)
	default:
		return fmt.Errorf("SetStringField: %s.%s type mismatch, expect bool, got [%s]", name, key, field.Type().Name())
	}
	return nil
}

//调用objPtr的方法
func CallMethod(objPtr interface{}, method string, args ...interface{}) []interface{} {
	value := reflect.ValueOf(objPtr)
	fn := value.MethodByName(method)
	if fn.IsValid() {
		var input []reflect.Value
		for _, arg := range args {
			input = append(input, reflect.ValueOf(arg))
		}
		output := fn.Call(input)
		var result []interface{}
		for _, out := range output {
			result = append(result, out.Interface())
		}
		return result
	} else {
		println("method not found", method)
	}
	return nil
}

//获取所有字段名称
func GetStructFieldNames(ptr interface{}) []string {
	var value = reflect.ValueOf(ptr).Elem()
	var st = value.Type()
	var names = make([]string, 0, st.NumField())
	for i := 0; i < st.NumField(); i++ {
		var field = st.Field(i)
		names = append(names, field.Name)
	}
	return names
}

//获取所有字段值
func GetStructFieldValues(ptr interface{}) []interface{} {
	var value = reflect.ValueOf(ptr).Elem()
	var st = value.Type()
	var values = make([]interface{}, 0, st.NumField())
	for i := 0; i < st.NumField(); i++ {
		var field = value.Field(i)
		values = append(values, field.Interface())
	}
	return values
}

//获取所有字段名称-值
func GetStructFieldKeyValues(ptr interface{}, tag string) map[string]string {
	var value = reflect.ValueOf(ptr).Elem()
	var st = value.Type()
	var result = make(map[string]string, st.NumField())
	for i := 0; i < st.NumField(); i++ {
		var field1 = st.Field(i)
		var field2 = value.Field(i)
		var name = field1.Name
		if tag != "" {
			var s = field1.Tag.Get(tag)
			if s != "" {
				name = s
			}
		}
		result[name] = fmt.Sprintf("%v", field2.Interface())
	}
	return result
}
