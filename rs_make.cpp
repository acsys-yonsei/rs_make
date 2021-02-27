#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <utility>
#include <vector>

using namespace std;

uint64_t v;
uint64_t unit_v;
int vt;

vector<vector<int>> block;
vector<vector<uint64_t>> edge;
vector<vector<uint64_t>> new_edge;
vector<vector<int>> new_vertex;

void tiling();

int main(int argc, char** argv) {
	v = stoull(argv[1]);
	ifstream config_file(argv[2]);
	ifstream csr_file(argv[3]);
	ofstream output_file(argv[4]);

	if (config_file.is_open() && csr_file.is_open()) {
		string line, row_index, col_index, tmp;
		stringstream ss, tt;

		// initialize
		getline(config_file, line);
		vt = stoi(line);
		unit_v = ceil((float)v / vt);
		for (int i = 0; i < vt; i++) {
			block.push_back(vector<int> ());
			new_edge.push_back(vector<uint64_t> ());
		}

		for (uint64_t i = 0; i < v; i++) {
			edge.push_back(vector<uint64_t> ());
			new_vertex.push_back(vector<int> ());
			for (int j = 0; j < vt; j++)
				new_vertex[i].push_back(-1);
		}

		// block load
		for (int i = 0; i < vt; i++) {
			getline(config_file, line);
			ss.clear();
			ss.str(line);
			while (getline(ss, tmp, ' '))
				block[i].push_back(stoi(tmp));
		}
		ss.clear();
		config_file.close();

		// csr parsing
		getline(csr_file, row_index);
		getline(csr_file, col_index);
		ss.str(row_index);
		tt.str(col_index);

		uint64_t idx = 0;
		uint64_t length = 0;
		uint64_t prev = 0;
		uint64_t cur;
		getline(ss, tmp, ' ');
		while (getline(ss, tmp, ' ')) {
			cur = stoull(tmp);
			length = cur - prev;
			for (uint64_t i = 0; i < length; i++) {
				getline(tt, tmp, ' ');
				edge[idx].push_back(stoull(tmp));
			}
			prev = cur;
			idx++;
		}

		csr_file.close();
		cout<<"vertex: "<<v<<", unit vertex: "<<unit_v<<endl;
		cout<<"Tiling Start"<<endl;
		// do rs
		tiling();

		int v_index = 0;
		int total_v = 0;
		output_file<<"0 ";
		for (int i = 0; i < vt; i++) {
			if (i % 2 == 0) {
				for (uint64_t j = 0; j < v; j++) {
					if (new_vertex[j][i] != -1) {
						v_index += new_vertex[j][i];
						total_v++;
						output_file<<v_index;
						if (i != vt-1 || j != v-1)
							output_file<<" ";
					}
					else
						continue;
				}
			}
			else
			{
				for (uint64_t j = 0; j < v; j++) {
					if (new_vertex[v -1 - j][i] != -1) {
						v_index += new_vertex[j][i];
						total_v++;
						output_file<<v_index;
						if (i != vt-1 || j != v-1)
							output_file<<" ";
					}
					else
						continue;
				}
			}
			
		}
		output_file<<endl;
		for (int i = 0; i < vt; i++) {
			if (i % 2 == 0) {
					for (uint64_t j = 0; j < new_edge[i].size(); j++) {
					output_file<<new_edge[i][j];
					if (i != vt-1 || j != new_edge[i].size() - 1)
						output_file<<" ";
				}
			}
			else {
				for (uint64_t j = 0; j < new_edge[i].size(); j++) {
					output_file<<new_edge[i][new_edge[i].size() - 1 - j];
					if (i != vt-1 || j != new_edge[i].size() - 1)
						output_file<<" ";
				}
			}
		}
		output_file<<endl;
		output_file.close();
		cout<<"Tiling end."<<endl;
		cout<<"Total v: "<<total_v<<", e: "<<v_index<<endl;
	}
}

void tiling() {
	for (uint64_t i = 0; i < v; i++) {
		int tile = i / unit_v;
		int idx = 0;
		uint64_t limit = 0;
		for (int j = 0; j < block[tile].size(); j++) {
			limit = idx + block[tile][j];
			limit *= unit_v;
			int accum = 0;
			while(!edge[i].empty() && edge[i].front() < limit) {
				accum++;
				new_edge[idx].push_back(edge[i].front());
				edge[i].erase(edge[i].begin());
			}
			new_vertex[i][idx] = accum;
			idx += block[tile][j];
		}
	}
}

/* config.txt format
	n (vt_num)
	row1_block_length1 row1_block_length2 ... (unit: vt)
	row2_block_length1 row2_block_length2 ...
	...
	rown_block_length1 rown_block_length2 ...
*/

/* run
./rs v config_file csr_file output_file
*/
