from jsonrpc import ServiceProxy
access = ServiceProxy("http://127.0.0.1:8332")
pwd = raw_input("Exit wallet passphrase: ")
access.walletpassphrase(pwd, 60)
