# EXAMPLE: create a dataset

Para:
$YCSB_SCRIPT is the path of binary file
$i is just a client ID

The parameter **-mode** specifies whether to create a dataset or run workload. 
Notice: the workload could run if and only if the dataset was created.

```
$ $YCSB_SCRIPT -db basic -threads 1 -P $YCSB_DIR/workloads/zipfread.spec -n $i -mode l ; echo "$OP $i "`date` >> $COMPLETE_TIME
```

# EXAMPLE: Run a test
```
$ $YCSB_SCRIPT -db basic -threads 1 -P $YCSB_DIR/workloads/zipfread.spec -n $i -mode r ; echo "$OP $i "`date` >> $COMPLETE_TIME
```
