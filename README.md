# A KV high-performance mini-database based on memory and C++17
**This project is inspired by Redis source code.**

# Command line tools
Developed command line tool **kvctl**.
value type:string
```shell
yunfei@ubuntu:~/MiniKV/build$ ./kvctl --key qjx --operate set --value world --ip 43.143.229.22
+--------+
| Status |
+--------+
| OK     |
+--------+
yunfei@ubuntu:~/MiniKV/build$ ./kvctl --key qjx --operate get --ip 43.143.229.22
+-------+
| Value |
+-------+
| world |
+-------+
```

value type:list
```shell
yunfei@ubuntu:~/MiniKV/build$ ./kvctl --key zyf --operate set --value hello --encoding list --ip 43.143.229.22
+--------+
| Status |
+--------+
| OK     |
+--------+
yunfei@ubuntu:~/MiniKV/build$ ./kvctl --key zyf --operate set --value world --encoding list --ip 43.143.229.22
+--------+
| Status |
+--------+
| OK     |
+--------+
yunfei@ubuntu:~/MiniKV/build$ ./kvctl --key zyf --operate get --ip 43.143.229.22
+-------+
| Value |
+-------+
| hello |
| world |
+-------+
```
search key using regex:
```shell
yunfei@ubuntu:~/MiniKV/build$ ./kvctl --keyRex .* --operate findkey --ip 43.143.229.22
+--------+
| Key    |
+--------+
| zyf    |
| yunfei |
| qjx    |
+--------+
```


# build
**Dependencies: grpc, protobuf, gflags**
In the project dir, do:
```shell
cd build && cmake .. && make
```
then you can get `kvserver` and `kvclient`.

# run
```shell
./kvserver
```

# Persistent data frame format
![avatar](/images/dataframe.svg)

# About
- This project is based on the gRPC framework to implement the memory-based KV cache middleware, which realizes the client and server respectively. Support list, string type KV cache, data snapshot and progressive Rehash.

- Communication between client and server is realized based on gRPC, and concurrency security is realized based on read-write lock; The client supports the addition, deletion, modification and query of keys.

- Support data snapshot, background thread timing persistence, based on user-defined data frame format; Service start automaticly to read snapshot.

- The underlying storage structure mimics the design of Redis hash table, solves hash conflicts with zipper method, and realizes automatic memory management with intelligent pointer.

- Imitate Redis to implement the expired key based on the expired hash table, and clean up the expired key through lazy deletion.

- Simulate the design of Redis double hash table, and implement progressive rehash after the background thread calculates the load factor regularly.