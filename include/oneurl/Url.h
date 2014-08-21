/*
* Copyright (c) 2011-2012
* @version 1.0.0
* @author Zhai Zhouwei
* @email nuoline@gmail.com
* @date 2012-5-29
* @description URL分析以及归一化对外接口头文件
*/
#ifndef _ONE_URL_H_
#define _ONE_URL_H_

#include "UrlParse.h"

#include <string>
#include <vector>
using namespace std;

extern bool IsIp;

//类接口，归一化的同时保存了URL分析结果
class oneurl {
  public:
    //构造函数
    oneurl();
  
    //析构函数
    ~oneurl();
    
    //URL归一化对外接口
    string CNormalize(const string str_in, const int codetype = 0);
    
    //URL归一化uniq
    //string CNormalizeUniq(const string str_in, const int codetype = 0);
    
    //URL解析接口
    bool ParseUrl(const string str_in);
    
    bool Parse(const string &str_in);
    
    //scheme
    string GetScheme() const {
      return ComponentString(parsed_.scheme);
    }
    
    //username
    string GetUsername()const {
      return ComponentString(parsed_.username);
    }
      
    //password
    string GetPassword() const {
      return ComponentString(parsed_.password);
    }
    
    //host
    string GetHost() const {
      string strhost = ComponentString(parsed_.host);
      while(strhost.compare(0,8,"www.www.") == 0)
         strhost = strhost.substr(4);
      return strhost;
    }
    
    //port
    string GetPort() const {
      return ComponentString(parsed_.port);
    }
    
    //path
    string GetPath() const {
      return ComponentString(parsed_.path);
    }
    
    //query
    string GetQuery() const {
      return ComponentString(parsed_.query);
    }
    
    //ref
    string GetRef() const {
      return ComponentString(parsed_.ref);
    }
    
    //是否动态页面
    bool IsDynamic();
    
    //获取域名
    string GetDomain() const;
    
    //路径深度
    int PathDepth();
    
    //

  private:
    // 归一化后的的url
    string spec_;
    //分析后的结果
    Parsed parsed_;
    //
    string ComponentString(const Component& comp) const 
     {    
         if (comp.len <= 0)      
            return string();
        return string(spec_, comp.begin, comp.len);
     }
     
};

//切分字符串函数
void Split(const string str_value,const string str_spliter,vector<string>& v_str_list);

//预处理URL,如无scheme,添加http://
std::string PreProcess(const char* url,int url_len);
//
void RemoveSurplusSlash(const char* path,char* pc_path,int len);

//函数接口，效率高
string Normalize(const char* spec,int spec_len,int codetype = 0);
#endif
