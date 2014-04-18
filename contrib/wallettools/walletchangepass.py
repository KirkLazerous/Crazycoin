from jsonrpc import ServiceProxy
access = ServiceProxy("http://127.0.0.1:8332")
pwd = raw_input("Exit old wallet passphrase: ")
pwd2 = raw_input("Exit new wallet passphrase: ")
access.walletpassphrasechange(pwd, pwd2)
