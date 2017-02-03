#include "farisvm.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

#include "mongo/client/dbclient.h"

#define CNTR 498970

using namespace std;

string string_trim(const string& str, const char* delim=" \t\r\n");
string string_trim(const string& str, const char* delim)
{
  const int p1(str.find_first_not_of(delim));
  if(p1 == string::npos){
    return string();
  }
  const int p2(str.find_last_not_of(delim));
  return str.substr(p1, p2 - p1 + 1);
}

int
main(int argc, char *argv[])
{
    farisvm vm;
        
    mongo::client::initialize();          

    const char CR = '\r';
    const char LF = '\n';

    static int counter = 0;
    static int all = 0;
    
    // add filtering rules
    // vm.add_rule("||example.com^index", "filter1.txt");
    // vm.add_rule(".swf|", "filter2.txt");

    mongo::client::initialize();

    try {

      mongo::DBClientConnection client;
      client.connect( "localhost" );

      /* database: test, collection: users */
      const string ns = "farisvm.url";
      
      cout << "--- find ---" << endl;
      auto_ptr<mongo::DBClientCursor> cursor =
	client.query(ns, mongo::BSONObj());

      std::vector<farisvm::match_result> result[CNTR];
      farisvm::query_uri query[CNTR];

      query[0].set_uri("https://www.google.com/", "http://referer.com/");
      query[1].set_uri("http://example.com/index.html", "http://referer.com/");
      query[2].set_uri("http://example.com/index.swf", "http://referer.com/");
      
      counter = 3;
      string url2;

      while(cursor->more()) {
	mongo::BSONObj p = cursor->next();
	mongo::OID oid = p["_id"].OID(); 
	string url = p["url"].str();
	// cout << counter << ":" << "url: " << url << endl;

	url2 = string_trim(string(url));
	query[counter].set_uri(url2, "http://referer.com/");
	counter++;
	all++;
	
	if(counter > CNTR-1)
	  break;
      }

      const string ns2 = "farisvm.adblock";
      
      cout << "--- find2 ---" << endl;
      auto_ptr<mongo::DBClientCursor> cursor2 =
	client.query(ns2, mongo::BSONObj());

      // counter = 0;
      while(cursor2->more()) {
	mongo::BSONObj p2 = cursor2->next();
	mongo::OID oid = p2["_id"].OID(); 
	std::string pattern = p2["pattern"].str();
	// cout << "pattern: " << pattern;

	string pattern2;
	
	for (std::string::const_iterator it = pattern2.begin();
	     it != pattern2.end(); ++it) {
	  if (*it != CR && *it != LF) {
	    pattern += *it;
	  }	  
	}
       
	//pattern2 = pattern;
	pattern2 = string_trim(string(pattern));
	all++;
      }

    vm.add_rule("||example.com^index", "filter1.txt");
    vm.add_rule(".swf|", "filter2.txt");
    vm.add_rule("||www.icrc.org", "filter1.txt");

    vm.match(result, query, CNTR-1);

    cout << "total: " << all << std::endl;
    
    for (int i = 0; i < 10; i++) {
        std::cout << query[i].get_uri() << std::endl;
        for (auto ret: result[i]) {

	  if(!ret.rule.empty() && !ret.file.empty())
	    {
	     std::cout << "  rule: " << ret.rule
                      << "\n  file: " << ret.file << std::endl;
	    }
        }
        // std::cout << std::endl;
    }
      
    } catch( const mongo::DBException &e ) {
      std::cout << "caught " << e.what() << std::endl;
    }                       

    return 0;
}
