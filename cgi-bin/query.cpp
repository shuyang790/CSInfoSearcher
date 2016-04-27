//
//  main.cpp
//  query
//
//  Created by Dora on 4/27/16.
//  Copyright © 2016 Dora. All rights reserved.
//

#include <cstdio>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

bool not_empty = 0;

struct Entry
{
    string univ, name;
    int rank;
    bool acm_fellow, ieee_fellow, funding;
};

vector<Entry> list;
struct EntryOut
{
    Entry entry;
    int count;
    int id; // for stable sort
};

vector<EntryOut> outputbuffer;

map <string, string> univ_names;

inline string trans_lower(const string& token)
{
    string res(token);
    std::transform(token.begin(), token.end(), res.begin(), ::tolower);
    return res;
}

bool cmp(EntryOut a, EntryOut b)
{
    if (a.count == b.count) return a.id<b.id;
    return a.count > b.count;
}

void print(Entry& entry)
{
    if (not_empty)
        cout << ",";
    cout<<"{";
    cout<<"\"rank\":"<<"\""<<entry.rank<<"\",";
    cout<<"\"univ\":"<<"\""<<univ_names[entry.univ]<<"\",";
    cout<<"\"name\":"<<"\""<<entry.name<<"\",";
    cout<<"\"acm_fellow\":"<<"\""<<entry.acm_fellow<<"\",";
    cout<<"\"ieee_fellow\":"<<"\""<<entry.ieee_fellow<<"\",";
    cout<<"\"funding\":"<<"\""<<entry.funding<<"\"";

    cout<<"}";
    return;
}

int main(int argc, char *argv[]){
    if (argc !=2)
    {
        cout<<"Error Input"<<endl;
        return -1;
    }

    fstream input;
    string token;

    input.open("./database/cs-overall-rank.txt");
    while (getline(input, token, '|')){
        string abbr = token;
        getline(input, token, '\r');
        string fullname = token;
        abbr += "-";
        char tmp[3]="0\0";
        for (int i=0; i<token.size(); i++)
            if (token[i] >= 'A' && token[i] <= 'Z'){
                tmp[0] = token[i];
                abbr += tmp;
            }
        univ_names[abbr] = fullname;
        getline(input, token, '\n');
    }
    input.close();

    univ_names[string("40-UCOB")] = string("University of Colorado-​Boulder");
    univ_names[string("40-UTA")] = string("University of Utah");
    univ_names[string("29-UMN")] = string("University of Minnesota-​Twin Cities");
    univ_names[string("34-RU")] = string("Rutgers, The State University of New Jersey-​New Brunswick");
    univ_names[string("34-UCH")] = string("University of Chicago");
    univ_names[string("40-WUSTL")] = string("Washington University in St. Louis");

    input.open("./database/index.txt");
    //input.open("../database/index.txt");
    while (getline(input, token, '-'))
    {
        Entry entry;
        entry.rank = atoi(token.c_str());
        string last = token;
        last += "-";
        getline(input, token, '|');
        entry.univ = last + token;
        getline(input, token, '|');
        entry.name = token;
        getline(input, token, '|');
        entry.acm_fellow = (token =="yes" ? 1:0);
        getline(input, token, '|');
        entry.ieee_fellow = (token =="yes" ? 1:0);
        getline(input, token, '\n');
        entry.funding = (token =="yes" ? 1:0);
        //cout<<entry.rank<<" "<<entry.univ<< " "<<entry.name<<" "<<entry.acm_fellow<<endl;
        list.push_back(entry);
    }

    vector<string> keywords;
    string str(argv[1]);
    istringstream ss(str);
    while (getline(ss, token, ' '))
        keywords.push_back(trans_lower(token));

    cout << "[";
    for (int i=0 ; i<list.size(); ++i)
    {
        EntryOut entryout;
        entryout.entry = list[i];
        entryout.count = 0;
        entryout.id = i;
        for (int j=0; j<keywords.size(); ++j)
            if ((trans_lower(list[i].name).find(keywords[j]))!= -1
                ||(trans_lower(univ_names[list[i].univ]).find(keywords[j]))!= -1
                ||(trans_lower(list[i].univ).find(keywords[j]))!= -1)
                entryout.count++;
        if (entryout.count >0) outputbuffer.push_back(entryout);
    }

    //sort based on hit count
    sort(outputbuffer.begin(), outputbuffer.end(), cmp);
    for (int i=0; i<outputbuffer.size(); ++i)
    {
        if (outputbuffer[i].count == keywords.size()) print(outputbuffer[i].entry);
        not_empty = 1;
    }
    cout<<"]";
    return 0;
}
