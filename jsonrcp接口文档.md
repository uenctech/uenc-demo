# jsonrcp接口文档
---------------------------

说明：jsonrpc标准为2.0，文档中的数据皆为测试数据



## 一、获取高度（get_height）

### 请求
```
{
  "jsonrpc": "2.0",
  "id": "1",
  "method": "get_height"
}
```

### 返回值

```
{
  "jsonrpc": "2.0",
  "id": "1",
  "result": {
    "height": "100"
  }
}
```

### 字段说明
```
请求：
无参数
响应：
height 区块高度
```



## 二、通过高度获取所有交易hash(get_txids_by_height)

### 请求
```
{
  "jsonrpc": "2.0",
  "id": "1",
  "method": "get_txids_by_height",
  "params": {
    "height": "170"
  }
}
```

### 返回值
```
{
    "id": "1",
    "result": [
        "73260833fd3618d4c397391e5564e0c08b2198c6721cb472d954bc529dc80888",
        "64011febf0df54e57d5bba1e49adbfb37162809a3b68428f83fec821fdd8b000",
        "7b24b5607565c57c414e714efdc76c338afcd7dcd0582229df6c163f6298aff7"
    ]
}
```

### 字段说明
```
请求：
height 区块高度
响应：
当前区块高度所有交易hash组成的json数组
```



## 三、根据地址获取余额（get_balance）

### 请求
``` 
{
  "jsonrpc": "2.0",
  "id": "1",
  "method": "get_balance",
  "params": {
    "address": "xxx"
  }
}
```

### 返回值
```
{
  "jsonrpc": "2.0",
  "id": "1",
  "result": {
    "balance": "100"
  }
}
```

### 字段说明
```
请求：
address  钱包地址
响应：
balance  钱包余额
```



## 四、根据交易hash获取交易详情(get_tx_by_txid)

### 请求
``` 
{
  "jsonrpc": "2.0",
  "id": "1",
  "method": "get_tx_by_txid",
  "params": {
    "hash": "73260833fd3618d4c397391e5564e0c08b2198c6721cb472d954bc529dc80888"
  }
}
```


### 返回值
```
{
    
    "jsonrpc": "2.0",
    "id": "1",
    "result": {
    	"hash": "73260833fd3618d4c397391e5564e0c08b2198c6721cb472d954bc529dc80888",
        "time": 1601199133751212,
        "vin": [
            "1vkS46QffeM4sDMBBjuJBiVkMQKY7Z8Tu"
        ],
        "vout": [
            {
                "address": "1KDSDiooaAdWB84YXFLkceV6xwkpEJCbmw",
                "value": "100.0"
            },
            {
                "address": "1vkS46QffeM4sDMBBjuJBiVkMQKY7Z8Tu",
                "value": "9982.0"
            }
        ]
  	}   
}
```

### 字段说明
```
请求：
hash  交易hash
响应：
hash  交易hash
time  时间戳
vin   交易转出地址
vout  交易转入地址
value 交易金额 
```



## 五、创建交易体(create_tx_message)

### 请求
```
{
  "jsonrpc": "2.0",
  "id": "1",
  "method": "create_tx_message",
  "params": {
    "from_addr": [{"addr":"1126G5Cjd6zM4NdqhDZmYSCyGMVshGraEF"}],
    "to_addr":[{"addr":"16psRip78QvUruQr9fMzr8EomtFS1bVaXk","value":"2.0"}],
    "fee":"0.5"
  }
}
```

### 返回值
```
{
    "id": "1",
    "jsonrpc": "2.0",
    "result": {
        "tx_data": "EKux4LzXs+wCIiIxMTI2RzVDamQ2ek00TmRxaERabVlTQ3lHTVZzaEdyYUVGMig0YzIzODExZWVjZTViNjgwN2Y1NWU3Yzg4NzhiZjQwYTdjMWU2YmVkQkYKRApANWJhZjNhMzk1YjM5MzMyMDRjODdkMzlhYmUzZWYxZGY3YmRkNGZhOWY5ZGU2ZjNlZGNkYzA3YTllMGI5YjY0ZBABQkYKRApANjFmMjQ3ZDAzYWUyNzkyYjE1NDg2ZGJkYzY2YjQ2OGVmYjRlNWRlM2QwMzZhNzg0MTIxMGNiYTMwZWFkOTEzZRABQkQKQgpAODA0ZmQ1YWE3NWVmYzMxMGQ3NDYwMzU3NGQyNmJhZmI3ZTM3YjE4NmEwYmNlYWQzNjkyNjQ2YzViYTBkNzhkZUooCICJehIiMTZwc1JpcDc4UXZVcnVRcjlmTXpyOEVvbXRGUzFiVmFYa0oqCMC1pIMlEiIxMTI2RzVDamQ2ek00TmRxaERabVlTQ3lHTVZzaEdyYUVGUkR7Ikdhc0ZlZSI6MTAwMDAwMCwiTmVlZFZlcmlmeVByZUhhc2hDb3VudCI6MywiVHJhbnNhY3Rpb25UeXBlIjoidHgifQ==",
        "tx_encode_hash": "c313e223b413a3b7b41a7b87bf8a7ca1e3fcfa5b46c42f183b4417482f9205a7"
    }
}
```

### 字段说明
```
请求：
from_addr       交易转出地址
to_addr			addr:交易转入地址 value:交易金额
fee             交易燃料费
响应：
tx_data         交易体(base64编码)
tx_encode_hash  交易体hash(待签名信息)
```



## 六、发送交易(send_tx)

### 请求
```
{
  "jsonrpc": "2.0",
  "id": "1",
  "method": "send_tx",
  "params": {
  "tx_data": "EKux4LzXs+wCIiIxMTI2RzVDamQ2ek00TmRxaERabVlTQ3lHTVZzaEdyYUVGMig0YzIzODExZWVjZTViNjgwN2Y1NWU3Yzg4NzhiZjQwYTdjMWU2YmVkQkYKRApANWJhZjNhMzk1YjM5MzMyMDRjODdkMzlhYmUzZWYxZGY3YmRkNGZhOWY5ZGU2ZjNlZGNkYzA3YTllMGI5YjY0ZBABQkYKRApANjFmMjQ3ZDAzYWUyNzkyYjE1NDg2ZGJkYzY2YjQ2OGVmYjRlNWRlM2QwMzZhNzg0MTIxMGNiYTMwZWFkOTEzZRABQkQKQgpAODA0ZmQ1YWE3NWVmYzMxMGQ3NDYwMzU3NGQyNmJhZmI3ZTM3YjE4NmEwYmNlYWQzNjkyNjQ2YzViYTBkNzhkZUooCICJehIiMTZwc1JpcDc4UXZVcnVRcjlmTXpyOEVvbXRGUzFiVmFYa0oqCMC1pIMlEiIxMTI2RzVDamQ2ek00TmRxaERabVlTQ3lHTVZzaEdyYUVGUkR7Ikdhc0ZlZSI6MTAwMDAwMCwiTmVlZFZlcmlmeVByZUhhc2hDb3VudCI6MywiVHJhbnNhY3Rpb25UeXBlIjoidHgifQ==",
  "tx_signature":"YjM5MzMyMDRjODdkMzlhYmUzZWYxZGY3YmR",	     	                                "public_key":"ApANWJhZjNhMzk1YjM5MzMyMDRjODdkMzlhYmUzZWYxZGY3YmRkNGZhOWY5ZGU2ZjNlZGNkYzA3YTllMGI5YjY0ZBABQkYKRApANjFmMjQ3ZDAzYWUyNzkyYjE1NDg2ZGJkYzY2YjQ2OGVmYjRlNWRlM2QwMzZhNzg0MTIxMGNiYTMwZWFkOTEz"
  }
}
```
### 返回值
```
{
  "jsonrpc": "2.0",
  "id": "1",
  "result": {
    "tx_hash": "73260833fd3618d4c397391e5564e0c08b2198c6721cb472d954bc529dc80888",
  }
}
```

### 字段说明
```
请求：
tx_data         交易体(base64编码)
tx_signature    交易体hash的签名信息
public_key      公钥(base64编码)
响应：
tx_hash         交易hash(可通过此hash查询完整交易信息)
```


