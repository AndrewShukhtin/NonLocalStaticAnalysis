#include "MESH.h"


namespace FINITE_ELEMENT
{

	MESH::MESH()
	{

	};
	
	void MESH::ReadMesh(const std::string filename)
	{
		_filename = filename;
		
		meshfile.open(filename);
		
		assert(meshfile);

		std::string text;
	
		int Ndim, Ne, type;
	
		meshfile >> text >> Ndim;
	
	// 	std::cout << text << " " << Ndim <<std::endl;
		
		meshfile >> text >> Ne;
		
	// 	std::cout << text << " " << Ne <<std::endl;
		
		meshfile >> type;
		
	// 	std::cout << "type: "  << type <<std::endl;
		
		if(type == 9)
		{
			_Elements.resize(Ne, 4);
		}
		else if(type == 23)
		{
			_Elements.resize(Ne, 8);
		}	
		
			
		for (int j = _Elements.cols() - 1; j >= 0; --j)
			meshfile >>  _Elements(0,j);
		

		
		int Ni;
		meshfile >> Ni;
		
	// 	std::cout << "Elements.row(0): " << std::endl;
	// 	std::cout << Elements.row(0) << std::endl;
		
		for (size_t i = 1; i < Ne; ++i)
		{
			meshfile >> type;
			for (int j = _Elements.cols() - 1; j > -1; --j)
				meshfile >>  _Elements(i,j);
			if(type == 28)
				meshfile >> text;
			meshfile >> Ni;
		}
		
	// 	std::cout << std::endl;
	// 	std::cout << "Elements: " << std::endl;
	// 	std::cout << Elements << std::endl;
		
		int Nn;
		meshfile >> text >> Nn;
		
	// 	std::cout << text << " " << Nn << std::endl;
		_Nodes.resize(Nn,Ndim);
		
		
		for (size_t i = 0; i < Nn; ++i)
			meshfile >> _Nodes(i,0) >>  _Nodes(i,1) >> Ni;
		
	// 	std::cout << std::endl;
	// 	std::cout << "Nodes:" << std::endl;
	// 	std::cout << Nodes << std::endl;
		
		int Nb;
		
		meshfile >> text >> Nb;
		
	// 	std::cout << text << " " << Nb <<  std::endl;
		
		_Bounds.resize(Nb);
		
		for(size_t i = 0; i < Nb; ++i)
		{
			
			meshfile >> text >> _Bounds[i].tag;
		
	// 		std::cout << text << " " << tag << std::endl;
			
			meshfile >> text >> Ne;
			
	// 		std::cout << text << " " << Ne << std::endl;
			
			meshfile >> _Bounds[i].type;
			
	// 		std::cout << "Type "<< type << std::endl;
			
			if(_Bounds[i].type == 3)
			{
				_Bounds[i].Elements.resize(Ne,2);
			}else if (_Bounds[i].type == 21)
			{
				_Bounds[i].Elements.resize(Ne,3);
			}	
			
	// 		std::cout << Bounds[i].cols() << std::endl;
			
			for(size_t k = 0; k < _Bounds[i].Elements.cols(); ++k)
			{
				meshfile >> _Bounds[i].Elements(0,k);
			}
			
	//  		std::cout << Bounds[i].row(0) << std::endl;

			for(size_t j = 1; j < Ne; ++j)
			{
				meshfile >> type;
				
				for(size_t k = 0; k < _Bounds[i].Elements.cols(); ++k)
						meshfile >> _Bounds[i].Elements(j,k);
				
			}
			
	//  std::cout << Bounds[i] << std::endl;
		
	}
	
// 		for(size_t i = 0; i < Elements.cols(); ++i)
// 		{
// 			std::cout << Nodes.row(Elements(0,i)) << " ";
// 		}
			
		meshfile.close();
	};
	
	const std::string &MESH::FileName() const {return _filename;};
	
	const Eigen::MatrixXd &MESH::Nodes() const {return _Nodes;}
	
	const Eigen::MatrixXi &MESH::Elements() const {return _Elements;};
	
	const std::vector<Bound> &MESH::Bounds() const {return _Bounds;};
	
	const int MESH::NodesPerElement() { return _Elements.cols();}
	
	const int MESH::NumberOfNodes() {return _Nodes.rows(); };
	
	const int MESH::NumberOfElements() {return _Elements.rows(); };
	
	const int MESH::NumberOfBounds() {return _Bounds.size();};
	
	MESH::~MESH()
	{
		//std::cout << "Mesh destructor" << std::endl;
	};
	
	std::ostream &operator <<(std::ostream &os, const std::vector<Bound> &Bounds)
	{
		for (int i = 0; i < Bounds.size(); ++i)
		{
			os << "Tag: " <<Bounds[i].tag << std::endl;
			os << "Type of Element: " << Bounds[i].type << std::endl;
			os << "Elements: \n" <<Bounds[i].Elements << std::endl;
			os << std::endl;
		}
	}
}
