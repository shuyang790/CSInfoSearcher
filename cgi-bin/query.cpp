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

const int maxlength = 200;
bool toomany = 0;
int cnt = 0;
const int csvNameIndex = 6;
const int csvUnviIndex = 8;
const int csvStartDate = 4;
string result = "";

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
    vector<int> fundings;
};

vector<EntryOut> outputbuffer;

map <string, string> univ_names;
map <string, string> univ_name2abbr;

map < string, map<string, int> > directions;

class CSVRow
{
public:
    std::string const& operator[](std::size_t index) const
    {
        return m_data[index];
    }
    std::size_t size() const
    {
        return m_data.size();
    }
    void readNextRow(std::istream& str)
    {
        std::string         line;
        std::getline(str, line);
        
        std::stringstream   lineStream(line);
        std::string         cell;
        
        m_data.clear();
        string last = "";
        while(std::getline(lineStream, cell, ','))
        {
            if (cell.size()==0 || (cell[cell.length()-1]!='\"')) 
                {
                    last= last + cell + ','; 
                    continue;
                }
            cell = last + cell;
            if (cell.size() >0 && cell[0] =='\"')cell.erase(0,1);
            if (cell.size() > 0) cell.erase(cell.size()-1,1);
            m_data.push_back(cell);
            last = "";
        }
    }
private:
    std::vector<std::string>    m_data;
};
vector<CSVRow> nsfFundings;

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}

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

string myitoa(int cnt){
    if (!cnt)
        return string("0");
    string res="";
    char a[3]="0\0";
    while (cnt > 0){
        a[0] = cnt % 10 + 48;
        res = string(a) + res;
        cnt /= 10;
    }
    return res;
}

void print(Entry& entry, vector<int>& fundings)
{
    if (cnt+1 > maxlength){
        toomany = 1;
        return;
    }
    cnt++;
    result += ",{";
    result += "\"rank\":\"";
    result += myitoa(entry.rank);
    result += "\",";
    result += "\"univ\":\""; result += univ_names[entry.univ]; result += "\",";
    result += "\"name\":\""; result += entry.name; result += "\",";
    result += "\"acm_fellow\":\""; result += myitoa(entry.acm_fellow); result += "\",";
    result += "\"ieee_fellow\":\""; result += myitoa(entry.ieee_fellow); result += "\",";
    result += "\"funding\":\""; result += myitoa(entry.funding); result += "\"";
    
    result += ",\"fundingDetails\": [";
    for (int i=0; i<fundings.size(); ++i)
    {
        if (i!=0) result +=",";
        result+="{";
        for (int j=0; j<nsfFundings[0].size(); ++j)
        {
            if (j!=0) result += ",";
            result += "\""+ nsfFundings[0][j] +"\":\"";
            result += nsfFundings[fundings[i]][j] +"\"";
        }
        result+="}";
    }
    result += "]";
    
    for (map< string, map<string, int> >::iterator it=directions.begin();
         it != directions.end(); it++) {
        if (it->second.find(entry.univ) != it->second.end()) {
            result +=  ",\"" ; result +=  it->first ; result +=  "\":";
            result +=  "\"" ; result +=  myitoa(it->second[entry.univ]) ; result +=  "\"";
        }
    }
    
    result += "}";
    return;
}

void read_direction(const char * filename, const char * dirname) {
    fstream input;
    string token;
    input.open(filename);
    map<string, int> cur;
    while(getline(input, token, '|')){
        int num = atoi(token.c_str());
        getline(input, token, '\n');
        if (token[token.size()-1] == '\r')
            token.erase(token.size()-1);
        string abbr = univ_name2abbr[token];
        cur[abbr] = num;
    }
    directions[string(dirname)] = cur;
    input.close();
}

int date2i(string date)
{
    int dd,mm,yy;
    sscanf(date.c_str(), "%d/%d/%d", &dd,&mm,&yy);
    return dd + mm*100 + yy*10000;
}

bool fundingCmp(CSVRow& a, CSVRow& b)
{
    return date2i(a[csvStartDate]) > date2i(b[csvStartDate]);
}

void readNSFFunding()
{
    std::ifstream       file("./database/nsfAwards/Awards.csv");
    CSVRow              row;
    //The first line is the entry name, and index 6 should be the Investigator's name
    while(file >> row)
    {
        nsfFundings.push_back(row);
    }
    // sort by start date
    sort(nsfFundings.begin()+1, nsfFundings.end(), fundingCmp);
}

int main(int argc, char *argv[]){
    if (argc !=2)
    {
        cout << "Error Input" << endl;
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
    univ_names[string("15-UMD")] = string("University of Maryland-​College Park");
    univ_names[string("63-UBS")] = string("University at Buffalo-​SUNY");
    
    for(map<string, string>::iterator it=univ_names.begin(); it!=univ_names.end(); it++)
        univ_name2abbr[it->second] = it->first;
    
    read_direction("./database/cs-artificial-intelligence-rank.txt", "Artificial Intelligence");
    read_direction("./database/cs-programming-language-rank.txt", "Programming Language");
    read_direction("./database/cs-system-rank.txt", "System");
    read_direction("./database/cs-theory-rank.txt", "Theory");
    
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
        entry.acm_fellow = (token =="True" ? 1:0);
        getline(input, token, '|');
        entry.ieee_fellow = (token =="True" ? 1:0);
        getline(input, token, '\n');
        entry.funding = (token =="True" ? 1:0);
        //result += entry.rank; result += " "; result += entry.univ; result +=  " "; result += entry.name; result += " "; result += entry.acm_fellow; result += endl;
        list.push_back(entry);
    }
    
    vector<string> keywords;
    string str(argv[1]);
    istringstream ss(str);
    while (getline(ss, token, ' '))
        keywords.push_back(trans_lower(token));
    
    readNSFFunding();
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
        if (entryout.count >0)
        {
            entryout.fundings.clear();
            for (int j=1; j<nsfFundings.size(); ++j)
            {
                int hitCount = 0;
                for (int k=0; k<keywords.size(); ++k)
                {
                    if (trans_lower(nsfFundings[j][csvNameIndex]).find(keywords[k]))
                        hitCount++;
                    if (trans_lower(nsfFundings[j][csvUnviIndex]).find(keywords[k]))
                        hitCount++;
                }
                if (hitCount == entryout.count || hitCount>=2) entryout.fundings.push_back(j);
            }
            outputbuffer.push_back(entryout);
        }
    }
    
    //sort based on hit count
    sort(outputbuffer.begin(), outputbuffer.end(), cmp);
    for (int i=0; i<outputbuffer.size(); ++i)
    {
        if (outputbuffer[i].count == keywords.size())
            print(outputbuffer[i].entry, outputbuffer[i].fundings);
    }
    result += "]";
    
    string cur = "[{\"len\":\"";
    cur += myitoa(cnt);
    cur += "\",";
    if (toomany)
        cur += "\"info\":\"tooMany\"}";
    else
        cur += "\"info\":\"good\"}";
    
    result = cur + result;
    cout << result;
    return 0;
}