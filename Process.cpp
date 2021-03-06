//  Process.cpp
//
//
//  Created by Emma on 12/09/2016.
//
//

#include "Process.hpp"

void ProcData::calDeal() {
	ifstream fin;
	string s;
	long long item[100000] = {0}; 
	fin.open("/Users/emma/Work/alg/item.txt",ios::in);
	//fin.open("/Users/emma/Work/alg/sitem.txt",ios::in);
	while (getline(fin,s)) {
		vector<long long> tokens = split(s,' ');
		item[tokens[0]] = tokens[1];
	}
	if (DEBUG) {
		for (long long i=0; i<ITEMNUM; ++i) {
			cout<<i<<","<<item[i]<<endl;
		}
	}
	fin.close();

	fin.open("/Users/emma/Work/alg/deal.txt",ios::in);
	//fin.open("/Users/emma/Work/alg/sdeal.txt",ios::in);
	while (getline(fin,s)) {
		vector<long long> tokens = split(s,' ');
		long long count = tokens[1];
		for (long long i=1; i<=count; ++i) {
			user[tokens[0]] += item[tokens[2*i]]*tokens[2*i+1];
		}
	}
	fin.close();
	if (DEBUG) {
		ofstream out;
		out.open("/Users/emma/Work/alg/proc.txt",ios::out|ios::trunc);
		for (long long i=0; i<USERNUM; ++i) {
			out<<i<<","<<user[i]<<endl;
		}
		out.close();
	}
}

void ProcData::calFriend() {
	ifstream fin;
	string s;
	fin.open("/Users/emma/Work/alg/user.txt",ios::in);
	//fin.open("/Users/emma/Work/alg/suser.txt",ios::in);
	while (getline(fin,s)) {
		vector<long long> tokens = split(s,' ');
		for (long long i=0; i<tokens[1]; ++i) {
			Friend *f = new Friend;
			f->uid = tokens[i+2];
			f->next = fri[tokens[0]].next;
			fri[tokens[0]].next = f;
		}
	}
	fin.close();
	if (DEBUG) {
                for (long long i=0; i<USERNUM; ++i) {
			Friend* f = &fri[i];
			while (f->next != NULL) {
                        	cout<<f->uid<<",";
				f = f->next;
			}
			cout<<f->uid<<endl;
                }
        }
}

void ProcData::calCycle() {
	vector<pair<long long,list<long long>>> result;
	for (long long i=0; i<USERNUM; ++i) {
		stack<long long> cycle;
		long long money = 0;
		list<long long> l;
		
		Friend* f = &fri[i];
		while (f!=NULL) {
			long long index = f->uid;
			cycle.push(index);
			l.push_back(index);
			money += user[index];
			f = f->next;
		}
		while (!cycle.empty()) {
			long long index=cycle.top();
			cycle.pop();
			Friend* f = fri[index].next;
			while (f != NULL) {
				index = f->uid;
				if (find(l.begin(), l.end(), index) != l.end()) {
					f = f->next;
					continue;
				}
				l.push_back(index);
				money += user[index];
				f = f->next;
			}
		}
		cout<<endl;
		l.sort();
		result.push_back(make_pair(money,l));
	}
	sort(result.begin(), result.end());
	cout<<"end!!!!!!!!"<<endl;
	ofstream out;
	out.open("/Users/emma/Work/alg/rank.txt",ios::out|ios::trunc);
	out<<"Rank"<<endl;
	long long i = 0;
	list<long long> last;
	if (DEBUG) {
		for (auto it=result.rbegin(); it!=result.rend(); ++it) {
			list<long long> l = it->second;
			if (last == l)
				continue;
			long long money = it->first;
			out<<++i<<": {";
			string separator = "";
			for (auto in=l.begin(); in!=l.end(); ++in) {
				out<<separator<<*in;
				separator = ",";
			}
			out<<"}: "<<money<<endl;
			last = l;
		}
	}
	long long sum = 0;
	for (auto it=result.rbegin(); it!=result.rend(); ++it) {
		list<long long> l = it->second;
		if (last == l)
			continue;
		if (i<100) {
			sum+=it->first;
		}
		++i;
		last = l;
	}
	out<<i<<"\t"<<sum;
	out.close();
}

vector<long long> ProcData::split(const string &s, char delim) {
	stringstream ss(s);
	string item;
	vector<long long> tokens;
	while (getline(ss, item, delim)) {
		tokens.push_back(atoi(item.c_str()));
	}
	return tokens;
}

int main(){
	ProcData pd;
	pd.calFriend();
	pd.calDeal();
	pd.calCycle();
	return 1;
}
