#ifndef DRAW_TABLE_H
#define DRAWTABLE_H
#include <vector>
#include <iostream>
#include <iomanip>
void drawLine(std::vector<int>& max,int columns) {  //画行线
	for (int i = 0; i < columns; i++) {
		std::cout << "+-";
		for (int j = 0; j <= max[i]; j++) {
			std::cout << '-';
		}
	}
	std::cout << '+' << std::endl;
}

void drawDatas(std::vector<int>& max, std::vector<std::vector<std::string>>& Str,
               std::vector<std::string>& D,int columns,int row) { //显示构造过程，状态转换矩阵
	drawLine(max,columns);
	for (int i = 0; i < D.size(); i++) {
		std::cout << "| " << std::setw(max[i]) << std::setiosflags(std::ios::left) << std::setfill(' ') << D[i] << ' ';
	}
	std::cout << '|' << std::endl;
	drawLine(max, columns);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < columns; j++) {
			std::cout << "| " << std::setw(max[j]) << std::setiosflags(std::ios::left) << std::setfill(' ');
			std::cout << Str[i][j] << ' ';
		}
		std::cout << '|'<< std::endl;
	}
	drawLine(max, columns);
}

void maxLenForEveryCol(std::vector<std::vector<std::string>>& datas, std::vector<int>& maxLen) {
    if (datas.empty()) return;
    maxLen.resize(datas[0].size());
    for (int i = 0; i < datas[0].size(); ++i) {
        for (int j = 0; j < datas.size(); ++j) {
            if (datas[j][i].size() > maxLen[i]) {
                maxLen[i] = datas[j][i].size();
            }
        }
        
    }
    return;
}

#endif