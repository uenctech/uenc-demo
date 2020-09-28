# 接口文档

## 一、网络请求规范

1. 所有网络请求需按照协议规范（数据格式）将数据组织好后，使用socket连接方式发送到主网节点，当主网节点收到请求后将按照同样协议规范（数据格式）返回给请求方，请求方需自行解析相关数据获得接口返回信息。

2. 请求协议中部分信息是以protobuf方式进行封装，封装后序列化后填充请求协议中进行发送。一般请求协议的名称以Req结尾，发送到主网后，主网后会返回Ack结尾的protobuf数据体进行回应。例如请求获得余额的子协议接口为GetAmountReq，则主网服务器收到该请求后的回应数据体为GetAmountAck。

3. 请求协议的数据格式如下：

   | 协议总长度 |     通用协议     | 校验和 | 结束标志位 |
   | :--------: | :--------------: | :----: | :--------: |
   |   4bytes   | 序列化后的字符串 | 4bytes |   4bytes   |

   - 协议总长度

     - 该字段4字节，记录接下来的“通用协议”+“校验和”+“结束标记”的总长度。假设通用协议100个字节，校验和4个字节，结束标记4个字节，则总长度为100 + 4 + 4 = 108字节。

   - 通用协议

     - 通用协议为profobuf格式，原型如下：

       ```
       message CommonMsg {
       	string version        = 1;
       	string type           = 2;
       	int32 encrypt         = 3;
       	int32 compress        = 4;
       	bytes data            = 5;
       }
       ```

       |   字段   |                             说明                             |
       | :------: | :----------------------------------------------------------: |
       | version  |                     版本，目前暂时可为空                     |
       |   type   | 子协议类型名称，根据实际业务需要填充接口名称，如”GetAmountReq” |
       | encrypt  |            是否加密，1为加密，0为不加密，默认为0             |
       | compress |            是否压缩，1为压缩，0为不压缩，默认为0             |
       |   data   | 子协议接口protobuf数据体序列化后的数据,请求时需要将子协议接口的protobuf类型数据序列化后，填充到data中后进行发送。 |

   - 校验和

     - 校验和长度为4字节，是通用协议序列化后，通过Adler32算法计算出来的校验和信息。

   - 结束标志位

     - 结束标记长度为4字节，其内容为7777777。

4. 将请求协议发送到主网后，主网也按照请求协议格式封装回传信息传送给请求方，请求方收到信息后：

   1. 根据协议总长度获得完整回传信息。
   2. 根据协议总长度获得通用协议、校验和以及结束标记。
   3. 根据校验和判断通用协议是否完整。
   4. 通过反序列化通用协议，获取子协议（接口）名称和子协议（接口）的数据内容。
   5. 根据通用协议中的压缩和加密字段，确定是否对子协议（接口）的数据内容进行解压缩或解密操作。
   6. 将子协议的数据内容按照子协议的protobuf格式进行反序列化，获得完整的子协议（接口）请求内容。


## 二、接口说明
### 1. 获取账户余额接口（GetAmountReq）

* 请求

   ```
   message GetAmountReq 
   {
       string version = 1;
       string address = 2;
   }
   ```

   |  字段   |         说明         |
   | :-----: | :------------------: |
   | version | 版本，目前暂时可为空 |
   | address |       钱包地址       |

* 响应

   ```
   message GetAmountAck 
   {
       string version 		= 1;
       sint32 code 		= 2;
       string description 	= 3;
   
       string address 		= 4;
       string balance 		= 5;
   }
   ```

   |    字段     |              说明               |
   | :---------: | :-----------------------------: |
   |   version   |      版本，目前暂时可为空       |
   |    code     | 返回0为成功, -1为钱包地址不可用 |
   | description |        返回值的文字描述         |
   |   address   |            钱包地址             |
   |   balance   |              余额               |


### 2. 获得区块列表接口（GetBlockInfoListReq）

* 请求

   ```
   message GetBlockInfoListReq
   {
       string version 				= 1;
       uint32 index 				= 2;
       uint32 count 				= 3;
   }
   ```

   |  字段   |  说明  |
   | :-----: | :----: |
   | version | 版本号 |
   |  index  |  索引  |
   |  count  |  数量  |

* 响应

   ```
   message GetBlockInfoListAck
   {
       string version 				= 1; 
       int32 code 					= 2; 
       string description 			= 3;
       uint32 top 					= 4; 
       uint32 txcount 				= 5; 
       repeated BlockInfoItem list = 6; 
   }
   
   message BlockInfoItem
   {
       string blockhash 			= 1;
       uint32 blockheight 			= 2;
       uint64 time 				= 3;
       string txHash 				= 4;
       repeated string fromAddr 	= 5;
       repeated string toAddr 		= 6; 
       string amount  				= 7; 
   } 
   
   ```

   |    字段     |     说明     |
   | :---------: | :----------: |
   |   version   |    版本号    |
   |    code     |  返回错误码 0 成功; -1 版本不兼容; -2 读取数据失败; -3 获得区块高度失败; -4 获得交易数量失败; -5 获得交易数量失败; -6 保留; -7 获得区块哈希值错误; -8 获得区块错误|
   | description | 返回错误信息 |
   |     top     |    总高度    |
   |   txcount   |  总交易笔数  |
   |    list     | 区块信息列表 |
   |  blockhash  |   区块哈希   |
   | blockheight |   区块高度   |
   |    time     |    时间戳    |
   |   txHash    |   交易哈希   |
   |  fromAddr   |  发起方地址  |
   |   toAddr    |  接收方地址  |
   |   amount    |    交易额    |



### 3. 获得区块详情请求（GetBlockInfoDetailReq）

* 请求

   ```
   message GetBlockInfoDetailReq
   {
       string version 		= 1;
       string blockhash 	= 2;
   }
   ```

   |   字段    |   说明   |
   | :-------: | :------: |
   |  version  |  版本号  |
   | blockhash | 交易hash |

* 响应

   ```
   message GetBlockInfoDetailAck
   {
       string version 								= 1;
       int32 code 									= 2; 
       string description 							= 3;
   
       string blockhash 							= 4;
       uint32 blockheight 							= 5;
       string merkleRoot 							= 6; 
       string prevHash 							= 7; 
       uint64 time 								= 8;
       string tatalAmount 							= 9; 
   
       repeated string signer 						= 10;
       repeated BlockInfoOutAddr blockInfoOutAddr	= 11;
   }
   
   message BlockInfoOutAddr
   {
       string addr 								= 1;
       string amount 								= 2; 
   }
   ```

   |       字段       |         说明          |
   | :--------------: | :-------------------: |
   |     version      |        版本号         |
   |       code       |      返回错误码  0 成功; -1 版本不兼容; -2 读取数据失败; -3 获得区块失败     |
   |   description    |     返回错误信息      |
   |    blockhash     |       区块哈希        |
   |   blockheight    |       区块高度        |
   |    merkleRoot    |       区块哈希        |
   |     prevHash     |     前置区块哈希      |
   |       time       |        时间戳         |
   |   tatalAmount    | 交易总额,不包括手续费 |
   |      signer      |        签名者         |
   | blockInfoOutAddr |        交易额         |
   |       addr       |      接收方地址       |
   |      amount      |       接收金额        |
   


### 4. 获得交易信息列表请求（GetTxInfoListReq）

* 请求

   ```
   message GetTxInfoListReq
   {
       string version 			= 1; 
       string addr 			= 2; 
       uint32 index 			= 3; 
       uint32 count 			= 4; 
   }
   ```

   |  字段   |   说明   |
   | :-----: | :------: |
   | version |  版本号  |
   |  addr   |   地址   |
   |  index  |   索引   |
   |  count  | 查询数量 |

* 响应

   ```
   message GetTxInfoListAck
   {
       string version 				= 1;
       int32 code 					= 2;
       string description 			= 3;
       repeated TxInfoItem list 	= 4; 
       uint32 total 				= 5; 
   }
   message TxInfoItem
   {
       TxInfoType type 			= 1;
       string txhash 				= 2; 
       uint64 time 				= 3; 
       string amount 				= 4; 
   }
   enum TxInfoType {
       TxInfoType_Unknown 			= 0;
       TxInfoType_Originator 		= 1; 
       TxInfoType_Receiver  		= 2; 
       TxInfoType_Gas 				= 3;
       TxInfoType_Award 			= 4; 
       TxInfoType_Pledge 			= 5;
       TxInfoType_RedeemPledge 	= 6; 
   }
   ```

   |          字段           |                 说明                  |
   | :---------------------: | :-----------------------------------: |
   |         version         |                版本号                 |
   |          code           |              返回错误码 0 成功; -1 地址长度不正确; -2 获得交易信息失败; -3 该地址无交易信息; -4 索引越界; -5 获得交易信息错误; |
   |       description       |             返回错误信息              |
   |          list           |             交易信息列表              |
   |          total          |            交易条目总数量             |
   |          type           | 交易类型，详见enum TxInfoType类型说明 |
   |         txhash          |               交易哈希                |
   |          time           |                时间戳                 |
   |         amount          |                交易额                 |
   |   TxInfoType_Unknown    |                 未知                  |
   |  TxInfoType_Originator  |              交易发起方               |
   |   TxInfoType_Receiver   |              交易接收方               |
   |     TxInfoType_Gas      |              手续费奖励               |
   |    TxInfoType_Award     |               区块奖励                |
   |    TxInfoType_Pledge    |                 质押                  |
   | TxInfoType_RedeemPledge |               解除质押                |



### 5. 获得交易详情接口（GetTxInfoDetailReq）

* 请求

   ```
   message GetTxInfoDetailReq
   {
       string version 	= 1; // 版本
       string txhash 	= 2; // 交易哈希
       string addr 	= 3; // 地址，传空值则不会查询针对该地址的奖励信息
   }
   ```

   |  字段   |                    说明                    |
   | :-----: | :----------------------------------------: |
   | version |                   版本号                   |
   | txhash  |                  交易哈希                  |
   |  addr   | 地址，传空值则不会查询针对该地址的奖励信息 |

* 响应

   ```
   message GetTxInfoDetailAck
   {
       string version 				= 1; 
       int32 code 					= 2; 
       string description 			= 3;
       string blockhash 			= 4; 
       uint32 blockheight 			= 5; 
       string txhash 				= 6;
       uint64 time 				= 7; 
       repeated string fromaddr 	= 8; 
       repeated string toaddr 		= 9; 
       string gas 					= 10; 
       string amount 				= 11;
       string awardGas 			= 12; 
       string awardAmount 			= 13;
   }
   ```

   |    字段     |     说明     |
   | :---------: | :----------: |
   |   version   |    版本号    |
   |    code     |  返回错误码 0 成功; -1 传入的交易哈希值为空; -2 获得区块错误; -3 获得区块信息错误 -4 区块错误;  |
   | description | 返回错误信息 |
   |  blockhash  |   区块哈希   |
   | blockheight |   区块高度   |
   |   txhash    |   交易哈希   |
   |    time     |    时间戳    |
   |  fromaddr   |   发起地址   |
   |   toaddr    |   接收地址   |
   |     gas     | 付出交易Gas  |
   |   amount    |    交易额    |
   |  awardGas   | 获得奖励Gas  |
   | awardAmount |   区块奖励   |




### 6. 质押列表请求（GetPledgeListReq）

* 请求

   ```
   message GetPledgeListReq 
   {
       string version 	= 1;
       string addr 	= 2;
       uint32 index 	= 3; 
       uint32 count 	= 4;
   }
   ```

   |  字段   |   说明   |
   | :-----: | :------: |
   | version |  版本号  |
   |  addr   | 查询地址 |
   |  index  |   索引   |
   |  count  | 查询数量 |

* 响应

   ```
   message GetPledgeListAck
   {
       string version 				= 1; 
       int32 code 					= 2; 
       string description 			= 3;
       repeated PledgeItem list 	= 4; 
       uint32 total 				= 5; 
   }
   message PledgeItem
   {
       string blockhash 			= 1; 
       uint32 blockheight 			= 2; 
       string utxo 				= 3; 
       string amount 				= 4; 
       uint64 time  				= 5; 
       string fromaddr 			= 6; 
       string toaddr 				= 7; 
       string detail 				= 8; 
   } 
   ```

   |    字段     |      说明      |
   | :---------: | :------------: |
   |   version   |     版本号     |
   |    code     |   返回错误码 0 成功; -1 地址为空; -2 获得质押信息错误; -3 该地址无质押; -4 索引越界;  |
   | description |  返回错误信息  |
   |    list     |  质押信息列表  |
   |    total    | 质押条目总数量 |
   |  blockhash  |    区块哈希    |
   | blockheight |    区块高度    |
   |    utxo     |      utox      |
   |   amount    |   质押资产值   |
   |    time     |     时间戳     |
   |  fromaddr   |    发起地址    |
   |   toaddr    |    接收地址    |
   |   detail    |    详情描述    |





### 7. 获得全网节点矿费接口（GetNodeServiceFeeReq）

* 请求

   ```
   message GetNodeServiceFeeReq 
   {
       string version 							= 1;
   }
   ```

   |  字段   |         说明         |
   | :-----: | :------------------: |
   | version | 版本，目前暂时可为空 |

* 响应

   ```
   message GetNodeServiceFeeAck 
   {
       string version 							= 1;
       sint32 code 							= 2;
       string description 						= 3;
   
       repeated ServiceFee service_fee_info 	= 4;
   }
   message ServiceFee
   {
   	string max_fee 		= 1;
   	string min_fee 		= 2;
   	string service_fee 	= 3;
       string avg_fee 		= 4;
   }
   ```

   |       字段       |          说明          |
   | :--------------: | :--------------------: |
   |     version      |  版本，目前暂时可为空  |
   |       code       |         0成功          |
   |   description    |    返回值的文字描述    |
   | service_fee_info | 节点矿费信息结构体数组 |
   |     max_fee      |   前一百块交易最大值   |
   |     min_fee      |   前一百块交易最小值   |
   |   service_fee    |     矿工设置的矿费     |
   |     avg_fee      |   前一百块交易平均值   |




### 8. 设置设备密码接口（SetDevPasswordReq）

* 请求

   ```
   message SetDevPasswordReq 
   {
       string version 	= 1;
       string old_pass = 2;
       string new_pass = 3;
   }
   ```

   |   字段   |         说明         |
   | :------: | :------------------: |
   | version  | 版本，目前暂时可为空 |
   | old_pass |        旧密码        |
   | new_pass |        新密码        |

* 响应

   ```
   message SetDevPasswordAck 
   {
       string version 		= 1;
       sint32 code 		= 2;
       string description 	= 3;
   }
   ```

   |    字段     |                             说明                             |
   | :---------: | :----------------------------------------------------------: |
   |   version   |                     版本，目前暂时可为空                     |
   |    code     | 0成功; -1参数错误; -2 密码不能为空; -3旧密码非法（特殊字符或长度超过8个字节）; -4 新密码非法（特殊字符或长度超过8个字节）; -5 新旧密码不能相同; -6 旧密码验证不正确; -7 未知错误 |
   | description |                       返回值的文字描述                       |



### 9. 设置矿工费请求接口（SetServiceFeeReq）

* 请求

   ```
   message SetServiceFeeReq
   {
   	string version 				= 1;			
   	string password				= 2;					
   	string service_fee 			= 3;					
   }
   ```

   |    字段     |         说明         |
   | :---------: | :------------------: |
   |   version   | 版本，目前暂时可为空 |
   |  password   |       矿机密码       |
   | service_fee |        设定值        |

* 响应

   ```
   message SetServiceFeeAck
   {
   	string version 				= 1;				
   	sint32  code 				= 2;					
   	string description 			= 3;					
   }
   ```

   |    字段     |                   说明                    |
   | :---------: | :---------------------------------------: |
   |   version   |           版本，目前暂时可为空            |
   |    code     | 0成功; -6 密码错误; -7 滑动条数值显示错误 |
   | description |             返回值的文字描述              |



### 10. 发起交易接口（主网请求）（CreateTxMsgReq）

![image-20200927173922300](interface.assets/image-20200927173922300.png)

#### 第一步:

* 请求

   ```
   message CreateTxMsgReq
   {
   	TxHeaderMsg txHeaderMsg 		= 1;
   	string from 					= 2;
   	string to 						= 3;
   	string amt 						= 4;	
   	string minerFees 				= 5;
   	string needVerifyPreHashCount 	= 6;
   }
   
   message TxHeaderMsg
   {
   	string version 					= 1;
   }
   ```

   |          字段          |               说明               |
   | :--------------------: | :------------------------------: |
   |      txHeaderMsg       |   TxHeaderMsg 通用交易头结构体   |
   |          from          |          发起方钱包地址          |
   |           to           |          接收方钱包地址          |
   |          amt           |             交易金额             |
   |       minerFees        | 发起交易方支付的单个签名的矿工费 |
   | needVerifyPreHashCount |            签名共识数            |
   |        version         |              版本号              |

* 响应

   ```
   message CreateTxMsgAck
   {
   	string       version        = 1;                    					
   	int32        code			= 2;					
   	string		 message		= 3;					
   	string 		 txData			= 4;					
   	string       txEncodeHash   = 5;						
   }
   ```
   
   |     字段     |                        说明                         |
   | :----------: | :-------------------------------------------------: |
   |   version    |                        版本                         |
|     code     | 0 为成功; -1 版本错误;  -2 参数错误 ; -3 utxo未找到 |
   |   message    |                      返回描述                       |
   |    txData    |                     交易信息体                      |
   | txEncodeHash |         交易信息体base64之后sha256的hash值          |



#### 第二步：

* 请求

   ```
   message TxMsgReq
   {
   	string       version        = 1;                     				
   	string		 serTx			= 2;					
   	string		 strSignature	= 3;					
   	string		 strPub			= 4;					
   	string       txEncodeHash   = 5;						
   }
   ```
   
   |     字段     |                             说明                             |
   | :----------: | :----------------------------------------------------------: |
   |   version    |                             版本                             |
   |    serTx     |                     为第一次响应的txData                     |
   | strSignature |                    base64编码后的签名信息                    |
|    strPub    | 发起方公钥，需要base64编码。如果调用C动态库生成的公钥，需要将十六进制的字符串还原成二进制，再base64编码 |
   | txEncodeHash |                  为第一次响应的txEncodeHash                  |
   
* 响应

   ```
   message TxMsgAck
   {
   	string       version        = 1;                  					
   	int32        code			= 2;						
   	string		 message		= 3;					
   	string       txHash         = 4;                      
   }
   ```
   
   |  字段   |                             说明                             |
   | :-----: | :----------------------------------------------------------: |
   | version |                             版本                             |
   |  code   | 0 为发起交易成功，交易即将开始广播; -1 版本错误; -2 反序列交易失败; -3 交易体无效; -4 数据库创建事务失败; -5 (未使用); -7 已经签名; -8 验证签名失败; -9 获取本节点签名费失败; -10 发起交易方支付的 GasFee 为 0; -11 未找到匹配的签名节点; |
| message |                           返回描述                           |
   | txHash  |                           交易hash                           |




###  11. 发起质押请求(主网请求)（CreatePledgeTxMsgReq）

#### 第一步：

* 请求

   ```
   message CreatePledgeTxMsgReq 
   {
   	string version 					= 1;
   	string addr 					= 2;
   	string amt  					= 3;
   	string needVerifyPreHashCount 	= 4;
   	string gasFees 					= 5;
   }
   ```

   |          字段          |   说明   |
   | :--------------------: | :------: |
   |        version         |  版本号  |
   |          addr          | 质押地址 |
   |          amt           | 质押金额 |
   | needVerifyPreHashCount | 签名个数 |
   |        gasFees         |  燃料费  |

* 响应

   ```
   message CreatePledgeTxMsgAck
   {
   	string       version      	= 1;
   	int32        code			= 2;
   	string		 description	= 3;
   	string 		 txData			= 4;
   	string       txEncodeHash   = 5;
   }
   ```

   |     字段     |                             说明                             |
   | :----------: | :----------------------------------------------------------: |
   |   version    |                            版本号                            |
   |     code     | 0 成功； -1 版本错误； -2 参数错误； -3 创建事务失败； -4  创建交易体失败 |
   | description  |                         返回错误信息                         |
   |    txData    |                          交易信息体                          |
   | txEncodeHash |              交易信息体base64之后sha256的hash值              |



#### 第二步：

* 请求

   ```
   message PledgeTxMsgReq
   {
   	string       version       	= 1;
   	string		 serTx			= 2;
   	string		 strSignature	= 3;
   	string		 strPub			= 4;
   	string       txEncodeHash   = 5;
   }
   ```

   |     字段     |                说明                |
   | :----------: | :--------------------------------: |
   |   version    |               版本号               |
   |    serTx     |             交易信息体             |
   | strSignature |              签名信息              |
   |    strPub    |             发起方公钥             |
   | txEncodeHash | 交易信息体base64之后sha256的hash值 |

* 响应

   ```
   message TxMsgAck
   {
   	string       version        = 1;                      				
   	int32        code			= 2;					
   	string		 message		= 3;						
   	string       txHash         = 4;                
   }
   ```

   |  字段   |                             说明                             |
   | :-----: | :----------------------------------------------------------: |
   | version |                            版本号                            |
   |  code   | 0 成功； -1 交易体类型错误； -2反序列化接收数据失败； -3 交易体不合法； -4 事务创建失败； -7  已经签名，无需重复签名； -8 验证签名失败； -9 节点未设置签名费； -10 发起方支付的燃油费为0； -11 找不到可签名的节点； -101 版本不兼容； -102 事务创建失败 |
   | message |                         返回错误信息                         |
   | txHash  |                           交易hash                           |



### 12. 解除质押（主网请求）CreateRedeemTxMsgReq）

#### 第一步：

* 请求

   ```
   message CreateRedeemTxMsgReq
   {
   	string   version    				= 1;						         
       string   addr 						= 2;                                     
   	string   amt  						= 3;                                     
   	string   needVerifyPreHashCount 	= 4;                    
   	string   gasFees 					= 5; 
       string 	 txHash						= 6;						  
   }
   ```

   |          字段          |     说明     |
   | :--------------------: | :----------: |
   |        version         |    版本号    |
   |          addr          | 解质押的地址 |
   |          amt           |  解质押金额  |
   | needVerifyPreHashCount |  共识数个数  |
   |        gasFees         |    燃料费    |
   |         txHash         |   交易hash   |

* 响应

   ```
   message CreateRedeemTxMsgAck
   {
   	string       version      	= 1;				
   	int32        code			= 2;					
   	string		 description	= 3;				
   	string 		 txData			= 4;						
   	string       txEncodeHash   = 5;					
   }
   ```

   |     字段     |                             说明                             |
   | :----------: | :----------------------------------------------------------: |
   |   version    |                            版本号                            |
   |     code     | 0 成功； -1 版本错误； -2 参数错误； -3 事务创建失败； -4 获取质押地址失败； -5 账号未质押； -6 获取块失败； -7 未找到块头； -8 获取质押的UTXO失败； -9 未找到质押的UTXO； -10 创建解质押交易失败； |
   | description  |                         返回错误信息                         |
   |    txData    |                          交易信息体                          |
   | txEncodeHash |              交易信息体base64之后sha256的hash值              |



#### 第二步：

* 请求

   ```
   message RedeemTxMsgReq
   {
   	string       version       	= 1;						
   	string		 serTx			= 2;					
   	string		 strSignature	= 3;					
   	string		 strPub			= 4;					
   	string       txEncodeHash   = 5;					
   }
   ```

   |     字段     |                说明                |
   | :----------: | :--------------------------------: |
   |   version    |               版本号               |
   |    serTx     |             交易信息体             |
   | strSignature |              签名信息              |
   |    strPub    |             发起方公钥             |
   | txEncodeHash | 交易信息体base64之后sha256的hash值 |

* 响应

   ```
   message TxMsgAck
   {
   	string       version        = 1;                    					
   	int32        code			= 2;					
   	string		 message		= 3;						
   	string       txHash         = 4;                        
   }
   ```

   |  字段   |                             说明                             |
   | :-----: | :----------------------------------------------------------: |
   | version |                            版本号                            |
   |  code   | 0 成功； -1 交易体类型错误； -2反序列化接收数据失败； -3 交易体不合法； -4 事务创建失败； -7  已经签名，无需重复签名； -8 验证签名失败； -9 节点未设置签名费； -10 发起方支付的燃油费为0； -11 找不到可签名的节点； -101 版本不兼容； |
   | message |                         返回错误信息                         |
   | txHash  |                           交易hash                           |


