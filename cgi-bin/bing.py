#!/usr/bin/python
# -*- coding: utf-8 -*-
import urllib
import urllib2
import requests
import json, re
import sys
reload(sys)
sys.setdefaultencoding("utf-8")

def getlinks(keyword, number=3):
    html = bing_search(keyword)
    length = len(html)
    results = []
    cur = 0
    for pos in re.finditer("<h2>", html):
        cur += 1
        if cur > number:
            break
        start = pos.start()
        while start < length and not html[start:].startswith('a href="'):
            start += 1
        start += len('a href="')
        end = start + 1
        while end < length and html[end] != '"':
            end += 1
        results.append(html[start:end])
    return results

def main():
    with open("./database/index.txt") as f:
        lines = f.readlines()
    f = open("./database/links.txt", "w")
    for line in lines:
        delim = line.find("|")
        delim2 = line[delim+1:].find("|") + delim
        keyword = line[delim+1:delim2].replace(" ", "%20")
        f.write(str(getlinks(keyword)) + "\n")
        f.flush()
    f.close()

def bing_search(keyword):
    url = "https://www.bing.com/search?q=" + keyword.replace(" ", "%20")
    headers = { "Accept":"text/html,application/xhtml+xml,application/xml;",
            "Accept-Encoding":"gzip",
            "Accept-Language":"zh-CN,zh;q=0.8",
            "Referer":"http://www.example.com/",
            "User-Agent":"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.90 Safari/537.36"
            }
    response = requests.get(url, headers=headers)
    return response.text



if __name__ == "__main__":
    main()
