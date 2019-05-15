#ifndef __ISOFEMSOL_H__
#define __ISOFEMSOL_H__

#include "MESH.h"
#include "QuadratureUtils.h"

#include <map>
#include <Eigen/Sparse>
#include <memory>
#include <functional>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <iomanip>


using Triplet = Eigen::Triplet<double>;

class ISOFEMSOL
{
public:
	ISOFEMSOL(std::string _filename, double _p1);
	
	ISOFEMSOL(std::string _filename, double _p1, double R);
	
	void Static_Analysis(std::function<Eigen::Vector2d(Eigen::RowVector2d &)> load);
	
	void NonLocal_Static_Analysis(std::function<Eigen::Vector2d(Eigen::RowVector2d &)> load, 
						std::function<double(Eigen::RowVector2d&, Eigen::RowVector2d&, const double &)> phi);
	
private:
	
	Eigen::SparseMatrix<double> K;
	std::vector<Triplet> TripletList;
	Eigen::MatrixXd	Ke,        // матирца жесткости элемента
					BT,        // оператор связи деформаций и перемещений (транспонированный)
					B;
	Eigen::VectorXd F;         // глобальный вектор правой части
	Eigen::VectorXd Fe;        // вектор правой части элемента
	Eigen::Matrix2d Jmatr;     // матрица Якоби в двумерном случае
	Eigen::RowVector2d Jvec;   // матрица Якоби в одномернмо случае
	Eigen::Matrix3d D;         // матрица Гука
	
	Eigen::VectorXd U;         // глобальный вектор Перемещений
	Eigen::VectorXd EpsiXX,  EpsiYY,  EpsiXY,  EpsiZZ;
	Eigen::VectorXd SigmaXX, SigmaYY, SigmaXY, SigmaZZ;
	
	std::map<int, int> cN;
	
	std::vector<std::vector<int>> RnnArr;
	
	double p1, p2;             // параметры вклада локальных и нелокальных эффектов
	
	double R;
	
	int NumberOfNodes, NumberOfElements, NodesPerElement, UxBoundNumber, 
		UyBoundNumber, UxUyBoundNumber, LoadBoundNumber;
	
	FINITE_ELEMENT::MESH MESH;
	
	void ConstructRightPart(std::function<Eigen::Vector2d(Eigen::RowVector2d &)> load);

	void ConstructStiffMatr();
	
	void ConstructStiffMatr(std::function<double(Eigen::RowVector2d&, Eigen::RowVector2d&, const double &)> phi);
	
	void ApplyingConstraints();
	
	std::vector<int> UniqueNodes(const Eigen::MatrixXi &Elements);
	
	inline void ComputeB(const Eigen::MatrixXd &Ndx);
	
 	inline void ComputeKe(const int &NumberOfQP, const Eigen::RowVectorXd &Weights, const Eigen::MatrixXd &ElementNodesCoord, const std::vector<Eigen::MatrixXd> &NGradArr, Eigen::MatrixXd &Ndx);
	
	inline void AddKeToTriplet(const Eigen::RowVectorXi &ElementNodesNumbers);
	
	void ComputeStrains();
	
	void ComputeStress();
	
	void ComputeStress(std::function<double(Eigen::RowVector2d&, Eigen::RowVector2d&, const double &)> phi);
	
	std::map<int, int> Counter(const Eigen::MatrixXi &);
	
	void NaiveRnnSearch(std::vector<std::vector<int>> &RnnArr, const double &R);
	
	void WriteToVTK(std::string _filename);
	
	
};

	


#endif
