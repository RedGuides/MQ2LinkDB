#pragma once
#include <string>
#include <vector>
#include <thread>

struct SearchResult
{
	std::string line;
	size_t textPos, textLength;

	SearchResult(std::string line, size_t start, size_t length)
		: line(std::move(line)), textPos(start), textLength(length) {}
};

const static std::vector<std::string> queryLinkByItemID(int itemID);
static std::vector<SearchResult> SearchLinkDB(const std::string& searchText, bool bExact, int limit = -1);
static std::vector<SearchResult> QueryLinkDB(const std::string& queryText);
static void UpdateItemsFromURL(const std::string& itemsURL);
static void ConvertItemsDotTxt();
static void DownloadLatestItemsTxt(const std::string& itemsURL);
const static void queryLinkCount();
const static void StoreLink(std::string_view link);
