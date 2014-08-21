#include "socket/socket.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <ctime>
#include <sstream>

#include "index/IndexConfig.h"
#include "index/Index.h"
#include "search/BooleanQuery.h"
#include "search/CustomQuery.h"
#include "search/VectorQuery.h"
#include "search/VectorQueryPageRank.h"
#include "search/BM25Query.h"
#include "search/BM25QueryPageRank.h"
#include "search/PageRank.h"
#include "search/PageRankQuery.h"
#include "utils/TimeProfiler.h"

using namespace std;

#define MAX_RESULTS 10
#define QUERIES_FILE "queries_data.txt"

/*
 * Shows how to use this program.
 */
void show_usage() {
	printf(
			"Usage: index_server -d <directory> -t <temporary file name>  -n <number of documents>\n");
}

/*
 * Gets command line arguments
 */
bool parse_arguments(int argc, char** argv, string &temporary_file_name,
		string &directory, size_t &documents_number) {

	bool t = false, d = false, n = false;

	// Parse the arguments from command line
	for (int i = 1; i < argc; i++) {
		string param(argv[i]);
		if (param == "--temporaryFileName" || param == "-t") {
			temporary_file_name = string(argv[++i]);
			t = true;
		} else if (param == "--directory" || param == "-d") {
			directory = string(argv[++i]);
			d = true;
		} else if (param == "--documentsNumber" || param == "-n") {
			n = true;
			if (sscanf(argv[++i], "%ld", &documents_number) == EOF)
				return false;
		}
	}

	if (t && d && n)
		return true;
	else
		return false;
}

string receive_request(int sd) {
	char r_msg[1000];
	recv_socket(sd, r_msg);
	//	send_socket(sd, (char*) SOCKET_OK);
	return string(r_msg);
}

void send_response(int sd, string json) {

	send_socket(sd, (char *) json.c_str());
	//	send_socket(sd, (char *) END_MSG);
}

const string currentDateTime() {
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}

void persist_query(string query) {
	FILE *fp = fopen(QUERIES_FILE, "a");
	string now = currentDateTime();
	fprintf(fp, "%s - %s", now.c_str(), query.c_str());
	fclose(fp);
}

void session(int sd, Index &index, DocFileManager &docFileManager,
		PageRank &pageRank) {

	TimeProfiler timeProfiler;

	//receive request
	string request(receive_request(sd));
	printf("Request: %s\n", request.c_str());

	// start timer
	timeProfiler.start();

	//process query
	unsigned pos = request.find_first_of(",");
	string model = request.substr(0, pos);
	string query = request.substr(pos + 1, request.npos);

	printf("Model %s\n", model.c_str());
	printf("Query %s\n", query.c_str());

	vector<term_info> result_list;

	ResultSet results;
	double time;

	if (model == "Boolean") {
		BooleanQuery booleanQuery(index, docFileManager);
		results = booleanQuery.query(query);
//	} else if (model == "BM15") {
//		BM25Query bm25Query(index, docFileManager, 0.0);
//		results = bm25Query.query(query);
	} else if (model == "BM25") {
		BM25Query bm25Query(index, docFileManager, 0.75);
		results = bm25Query.query(query);
	} else if (model == "BM25 + PageRank") {
		BM25QueryPageRank bm25QueryPageRank(index, docFileManager, 0.75,
				pageRank, 0.3);
		results = bm25QueryPageRank.query(query);
	} else if (model == "Cosine") {
		VectorQuery vectorQuery(index, docFileManager, NORM_VECTOR);
		results = vectorQuery.query(query);
	} else if (model == "Cosine + PageRank") {
		VectorQueryPageRank vectorQueryPageRank(index, docFileManager,
		NORM_VECTOR, pageRank, 0.3);
		results = vectorQueryPageRank.query(query);
	} else if (model == "PageRank") {
		PageRankQuery pageRankQuery(index, docFileManager, pageRank);
		results = pageRankQuery.query(query);
	} else if (model == "BM25 + Cosine") {
		CustomQuery customQuery(index, docFileManager);
		results = customQuery.query(query);
	}

	string json_r = "";

	int i = 0;
	while (results.hasNext() && i < MAX_RESULTS) {
		DocInfo docInfo = results.next();
		json_r.append(
				"{\"title\":\"" + docInfo.getTitle() + "\",\"url\":\""
						+ docInfo.getUrl() + "\"}");

		i++;
		if (results.hasNext() && i < MAX_RESULTS) {
			json_r.append(",");
		}
	}

	ostringstream strs;
	strs << results.getSize();
	string size = strs.str();

	char time_buf[20];
	time = timeProfiler.reportTime();
	sprintf(time_buf, "%4.3g", time);
	string time_str(time_buf);

	string json = "{\"size\":" + size + ",\"time\":\"" + time_str
			+ "\",\"data\":[" + json_r + "]}\r\n";

	send_response(sd, json);

	persist_query(query);
//	printf("Response: %s\n", json.c_str());
}

int main(int argc, char **argv) {

	string directory;
	string temporary_file_name;
	int sd, sd_current;
	size_t documents_number;

	/* open connection */
	open_server(&sd);

	// Parse the command line arguments
	if (!parse_arguments(argc, argv, temporary_file_name, directory, documents_number)) {
		show_usage();
		exit(EXIT_FAILURE);
	}

	// Set up index configurations

	IndexConfig config;
	config.setDirectory(directory);
	config.setTmpFileName(temporary_file_name);
	config.setDictFileName("dict");
	config.setIndexFileName("index-compressed");
	config.setDocFileName("docInfo");
	config.setDocIndexFileName("docInfoIndex");
	config.setDocumentsMaxNumber(documents_number);

	//	PageRank
	string outPath = config.getDirectory() + "outLinks";
	string inPath = config.getDirectory() + "inLinks";
	PageRank pageRank(outPath, inPath, config.getDocumentsMaxNumber(), 0.45);

	// Build index
	Index index(config);

	DocFileManager docFileManager(config.getDocFilePath(), "r");
	docFileManager.loadIndex(config.getDocIndexFilePath());

	printf("Server is ready!\n\n");

	// TODO: paralelize
	while (1) {
		/* wait for a client */
		accept_socket(&sd, &sd_current);

		/* session */
		session(sd_current, index, docFileManager, pageRank);
	}

	/* close socket */
	close(sd);

	return 0;
}
