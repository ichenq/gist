
### 结构
* 数据类型的结构定义(来自excel, SQL, xml, lua, json)
* 对应具体语言的struct生成
* 辅助的标签
* 文件加载代码

### 数据

* 数据读取，不同的数据源，csv, xml, json, lua
* 数据写入，不同格式转换
* 数据项查找，生成map索引


### 实现

#### Excel

* 先读取excel的@meta，可以指定一个单独的meta文件，需要包含文件路径


