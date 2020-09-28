extern "C" {
    /**
        说明:
            生成公私钥和钱包地址
        参数:
            ver: 版本号
                传0生成1开头的base58地址
                传5生成3开头的base58地址
        返回值:
            私钥,公钥,base58地址 拼接而成的字符串 以空格衔接
            私钥,公钥为十六进制字符串
    */
    char* GenWallet(int ver);

    /**
        说明:
            生成签名
        参数:
            pri: 十六进制的私钥
            msg: 待签名信息
            len: 待签名信息的长度
        返回值:
            经过base64编码之后的签名信息
    */
    char* GenSign(char* pri, char* msg, int len);
}
