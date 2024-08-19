//
//  basic_db.h
//  YCSB-C
//
//  Created by Jinglei Ren on 12/17/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef YCSB_C_BASIC_DB_H_
#define YCSB_C_BASIC_DB_H_

#include "core/db.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <mutex>
#include "core/properties.h"
#include <sys/stat.h>
#include<fcntl.h> 
#include <sys/types.h>
#include <sys/time.h>
#include <fstream>

using namespace std;

namespace ycsbc {

class BasicDB : public DB {
 public:
  int clientNum = 0;
  string workPath;
  string timeFilePath;
  ofstream timeCollector;
  BasicDB(int clientNumPara, string workPathPara){
    clientNum = clientNumPara;
    workPath = workPathPara+to_string(clientNum)+"/ycsbzipf/usertable"+to_string(clientNum);
  	timeFilePath = "./timeCollector/client"+to_string(clientNum);
  }

  void Init() {

    std::lock_guard<std::mutex> lock(mutex_);
    cout << workPath <<endl;
	if(timeCollector){
		timeCollector.close();
	}
	timeCollector.open(timeFilePath,ios::out|ios::trunc);
    mkdir(workPath.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
    //cout << "A new thread begins working." << endl;
  }

  int Read(const std::string &table, const std::string &key,
           const std::vector<std::string> *fields,
           std::vector<KVPair> &result) {
    std::lock_guard<std::mutex> lock(mutex_);
    //cout << "READ " << table << clientNum << ' ' << key << endl;
    timeval opStart, opEnd;
	double timeCostMs;
	ifstream readFile;
	//char _buffer[4000];
	gettimeofday(&opStart, NULL);
    readFile.open(workPath+"/"+key, ios::in);
	//readFile>>_buffer;
	gettimeofday(&opEnd, NULL);
	timeCostMs = 1000*(opEnd.tv_sec-opStart.tv_sec)+(opEnd.tv_usec-opStart.tv_usec)/1000.0;
    //cout << "Read cost: " << timeCostMs <<endl;
	timeCollector <<  "Read cost: " << timeCostMs << "ms" << endl;
	readFile.close();
    /*if (fields) {
      cout << " [ ";
      for (auto f : *fields) {
        cout << f << ' ';
      }
      cout << ']' << endl;
    } else {
      cout  << " < all fields >" << endl;
    }*/
    return 0;
  }

  int Scan(const std::string &table, const std::string &key,
           int len, const std::vector<std::string> *fields,
           std::vector<std::vector<KVPair>> &result) {
    std::lock_guard<std::mutex> lock(mutex_);
    cout << "SCAN " << table << ' ' << key << " " << len;
    if (fields) {
      cout << " [ ";
      for (auto f : *fields) {
        cout << f << ' ';
      }
      cout << ']' << endl;
    } else {
      cout  << " < all fields >" << endl;
    }
    return 0;
  }

  int Update(const std::string &table, const std::string &key,
             std::vector<KVPair> &values) {
    std::lock_guard<std::mutex> lock(mutex_);
    cout << "UPDATE " << table << ' ' << key << " [ ";
    for (auto v : values) {
      cout << v.first << '=' << v.second << ' ';
    }
    cout << ']' << endl;
    return 0;
  }

  int Insert(const std::string &table, const std::string &key,
             std::vector<KVPair> &values) {
    std::lock_guard<std::mutex> lock(mutex_);
    //cout << "INSERT " << table << clientNum << ' ' << key << endl ;
    timeval opStart, opEnd;
	double timeCostMs;
    ofstream insertFile;
    char _buffer[4000]={0};
    memset(_buffer,'$',3999);
	gettimeofday(&opStart, NULL);
    insertFile.open(workPath+"/"+key, ios::out);
    insertFile << _buffer << endl;
	gettimeofday(&opEnd, NULL);
	timeCostMs = 1000*(opEnd.tv_sec-opStart.tv_sec)+(opEnd.tv_usec-opStart.tv_usec)/1000.0;
    //cout << "Insert cost: " << timeCostMs <<endl;
	timeCollector << "Insert cost: " << timeCostMs << "ms" << endl;
    insertFile.close();
    /*for (auto v : values) {
      cout << v.first << '=' << v.second << ' ';
    }
    cout << ']' << endl;*/
    return 0;
  }

  int Delete(const std::string &table, const std::string &key) {
    std::lock_guard<std::mutex> lock(mutex_);
    cout << "DELETE " << table << ' ' << key << endl;
    return 0; 
  }

 private:
  std::mutex mutex_;
};

} // ycsbc

#endif // YCSB_C_BASIC_DB_H_

