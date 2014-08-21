CC = g++
INCLUDE_DIR=include/
LIB_DIR=lib/
LIBS=-lRICPlib -lz -lgumbo -lcmph -loneurl -licuuc -licudata -ldl

CPPFLAGS=-I$(INCLUDE_DIR) -std=c++0x
LDFLAGS=-L$(LIB_DIR) $(LIBS)

PREPROCESS = irsearch_preprocess
QUERY = irsearch_query
SERVER = irsearch_server

CPPSOURCES = src/analysis/Tokenizer.h \
src/analysis/Tokenizer.cpp \
src/commons/DocInfo.h \
src/commons/DocInfo.cpp \
src/commons/Lexicon.h \
src/commons/Lexicon.cpp \
src/commons/Record.h \
src/commons/Record.cpp \
src/compression/EliasGamma.h \
src/compression/EliasGamma.cpp \
src/compression/md5.h \
src/compression/md5.cpp \
src/index/hash.h \
src/index/hash.cpp \
src/index/Index.h \
src/index/Index.cpp \
src/index/IndexBuilder.h \
src/index/IndexBuilder.cpp \
src/index/IndexConfig.h \
src/index/IndexConfig.cpp \
src/index/IndexSorter.h \
src/index/IndexSorter.cpp \
src/index/IndexWriter.h \
src/index/IndexWriter.cpp \
src/parser/HTMLParser.h \
src/parser/HTMLParser.cpp \
src/search/Query.h \
src/search/Query.cpp \
src/search/BooleanQuery.h \
src/search/BooleanQuery.cpp \
src/search/BM25Query.h \
src/search/BM25Query.cpp \
src/search/CustomQuery.h \
src/search/CustomQuery.cpp \
src/search/VectorQuery.h \
src/search/VectorQuery.cpp \
src/search/ResultSet.h \
src/search/ResultSet.cpp \
src/search/BM25QueryPageRank.h \
src/search/BM25QueryPageRank.cpp \
src/search/VectorQueryPageRank.h \
src/search/VectorQueryPageRank.cpp \
src/search/PageRankQuery.h \
src/search/PageRankQuery.cpp \
src/search/PageRank.h \
src/search/PageRank.cpp \
src/socket/socket.h \
src/socket/socket.cpp \
src/sort/Heap.h \
src/sort/Heap.cpp \
src/sort/Quicksort.h \
src/sort/Quicksort.cpp \
src/utils/DictFileManager.h \
src/utils/DictFileManager.cpp \
src/utils/DocFileManager.h \
src/utils/DocFileManager.cpp \
src/utils/FileManager.h \
src/utils/FileManager.cpp \
src/utils/TimeProfiler.h \
src/utils/TimeProfiler.cpp


all: install
	@echo "Compiling index_writer_sorter"
	@$(CC)  src/irsearch_preprocess.cpp $(CPPSOURCES) $(CPPFLAGS) $(LDFLAGS) -o $(PREPROCESS)
	@echo "Compiling index_query"
	@$(CC)  src/irsearch_query.cpp $(CPPSOURCES) $(CPPFLAGS) $(LDFLAGS) -o $(QUERY)
	@echo "Compiling index_server"
	@$(CC)  src/irsearch_server.cpp $(CPPSOURCES) $(CPPFLAGS) $(LDFLAGS)  -o $(SERVER)

install:
	ln -f -s lib/libcmph.so.0.0.0 lib/libcmph.so
	ln -f -s lib/libgumbo.so.1.0.0 lib/libgumbo.so
	ln -f -s lib/libz.so.1.2.8 lib/libz.so

clean:
	$(RM) $(PREPROCESS) $(QUERY) $(SERVER) *.o
	clear