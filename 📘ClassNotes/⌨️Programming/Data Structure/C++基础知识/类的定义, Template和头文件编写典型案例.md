`fun_assertion.h`

```cpp 
#pragma once
#ifndef FUN_ASSERTION
#define FUN_ASSERTION
#include <iostream>;
#include <string.h>;
#include <assert.h>;    // this package is used for raise error
#include <math.h>;

using namespace std;

// The definition of the class should be put here. 

#pragma region Extend DataType
#pragma region DyArray
///  dynamic Array
template <typename ItemType>
class DyArray {
public:
	int length;
	DyArray() = default;
	DyArray(const int length, ItemType InitValue) {
		this->length = length;
		this->Array = new ItemType[length];
		for (int i = 0; i < length; i++) {
			*(this->Array + i) = InitValue;
		}
	};
	DyArray(const int length,ItemType InitArray[]) {
		this->length = length;
		this->Array = new ItemType[length];
		for (int i = 0; i < length; i++) {
			*(this->Array + i) = InitArray[i];
		}
	};
	void set(int index, ItemType value) {
		try {
			*(this->Array + index) = value;
		}
		catch (runtime_error &e) {
			throw "an error occurred!";
			cout << e.what() << endl;
		}
	}
	ItemType get(int index) {
		return *(this -> Array + index);
	}

	void Display_Content() {
		for (int i = 0; i < this -> length; i++) {
			cout << *(this->Array + i) << " ";
		}
		cout << "\n";
	}

	ItemType sum() {
		ItemType _sum = (ItemType)0;
		for (int i = 0; i < this->length; i++) {
			_sum += *(this->Array + i);
		}
		return _sum;
	}

private:
	ItemType* Array;
};

DyArray<int> Apply_fixed_constraint(DyArray<int> freedof, int nelx, int nely);
#pragma endregion

#pragma region DyMat
/// ------Dynamic Matrix-------
template <typename ItemType>
class DyMat {  // global parameters definition
	// in order to force the compiler to generate a default constructor one can use: 
    // <classname> { public: <classname>() = default; }
	public:
		int xelm;
		int yelm;
		DyMat() = default;  // force the complier to generate a default constructor

		DyMat(int xelm, int yelm, ItemType InitValue){
			//  initialize an array of dynamic size
			this->xelm = xelm;
			this->yelm = yelm;
			this->Mat = new ItemType* [xelm];
			for (int i = 0; i < xelm; i++) {
				*(this -> Mat + i) = new ItemType[yelm];
			}
			// attention the initialization of the Matrix with a userdefined boudary should be placed in initialization function
			// define a pointer which is for a 2-D array -> 
			// in this case we can initialize an 2-D array with predefined-size
			// ------------------initialize the value of the matrix ---------------------
			for (int i = 0; i < xelm; i++) {
				for (int j = 0; j < yelm; j++) {
					*(*(this -> Mat + i) + j) = InitValue;   // initialize the corresponding element of the pointer array
				}
			}
		};

		DyMat(int xelm, int yelm, ItemType *InitialMatrix) {
			/// attention if we want to use this Initilization method, we have to use forced type convert
			/// for example --->   DyMat<double> KE(8, 8, (double*)Ke );
			this->xelm = xelm;
			this->yelm = yelm;
			this->Mat = new ItemType* [xelm];  // pointer_array
			for (int i = 0; i < xelm; i++) {
				*(this->Mat + i) = new ItemType[yelm];
			}

			for (int i = 0; i < xelm; i++) {
				for (int j = 0; j < yelm; j++){
					*(*(this->Mat + i) + j) = *(InitialMatrix + i * yelm + j);
				}
			}
		};

		void set(int i, int j, ItemType value) {
			try {
				*(*(this->Mat + i) + j) = value;
			}
			catch (runtime_error& e) {
				throw "the index out of boundary";
				cout << e.what();
			}
		}
		// This funciton is used to set a peice of chunk;
		void setChunk(int lines[], int cols[], DyMat<ItemType> dmatrix ) {
			// there we initilize the lines[] as a pointer --> so we can't get it's size
			int lineNum = dmatrix.xelm;
			int colNum = dmatrix.yelm;
			for (int i = 0; i < lineNum ; i++) {
				for (int j = 0; j < colNum ; j++){
					int x_i = lines[i]; int y_i = cols[j];
					*(*(this->Mat + x_i)+ y_i) = dmatrix.get(i,j);
				}
			}
		}
		void setChunk(DyArray<int> lines, DyArray<int> cols, DyMat<ItemType> dmatrix) {
			for (int i = 0; i < lines.length; i++) {
				for (int j = 0; j < cols.length; j++) {
					int x_i = lines.get(i); int y_i = cols.get(j);
					*(*(this->Mat + x_i) + y_i) = dmatrix.get(i, j);
				}
			}
		}
		// void setChunk(DyArray<int> )

		DyMat<ItemType> getChunk(const int lines[],const int cols[],const int lineNum,const int colNum) {

			DyMat<ItemType> NewDM(lineNum, colNum,(ItemType)0 );
			// attention the null (0) should also be converted into ItemType to prevent the call of reload function
			try {
				for (int i = 0; i < lineNum; i++) {
					for (int j = 0; j < colNum; j++) {
						NewDM.set(i, j, *(*(this->Mat + lines[i]) + cols[j]));
					}
				}
				return NewDM;
			}

			catch(runtime_error &e){
				throw "A Unexpected fetal happened";
				cout << "Fetal: Unexpected fetal";
			}
		}

		DyMat<ItemType> getChunk(DyArray<int> lines, DyArray<int> cols) {
			// this function is for get Chunk by the DyArray
			DyMat<ItemType> NewDM(lines.length, cols.length,(ItemType)0);
			for (int i = 0; i < lines.length; i++) {
				for (int j = 0; j < cols.length; j++) {
					NewDM.set(i, j, *(*(this->Mat + lines.get(i)) + cols.get(j)));
				}
			}
			return NewDM;
		}

		ItemType get(const int i,const int j) {
			return *(*(this->Mat + i) + j);
		}

		void Display_Content(){
			for (int i = 0; i < this->xelm; i++) {
				for (int j = 0; j < this->yelm; j++) {
					cout << *(*(this->Mat + i) + j) << " ";
				}
				cout << "\n";
			}
		};
	private:
		ItemType** Mat;
};

DyMat<double> Elm_Stiff_Generate(double E, double nu);
#pragma endregion

#pragma endregion

/// this can store all the data needed in a class,
/// which is convenient for the exchange of data between different functions
#pragma region ParameterTypes

class FEM_parameters {
public:
	FEM_parameters(DyMat<double> k, DyMat<double> f, DyMat<double> u, DyArray<int> alldofs){
		this->K_mat = k;
		this->F_mat = f;
		this->U_mat = u;
		this->alldofs = alldofs;
	}
	DyMat<double>K() {
		return this->K_mat;
	}
	DyMat<double>F() {
		return this->F_mat;
	}
	DyMat<double>U() {
		return this->U_mat;
	}
	DyArray<int> Boundaries() {   
		// note that boundary conditions are predefined and can't be changed during the solution
		return this->alldofs;
	}
	void set_K(DyMat<double> K_new) {
		this->K_mat = K_new;
	}
	void set_F(DyMat<double> F_new) {
		this->F_mat = F_new;
	}
	void set_U(DyMat<double> U_new) {
		this->U_mat = U_new;
	}
	void Show_Data() {
		cout << "=============Data-Content=========================" << endl;
		cout << "-------------- K: --------------------------------" << endl;
		this->K_mat.Display_Content();
		cout << "-------------- F: --------------------------------" << endl;
		this->F_mat.Display_Content();
		cout << "-------------- U: --------------------------------" << endl;
		this->U_mat.Display_Content();
		cout << "-------------- boundary conditions ---------------" << endl;
		this->alldofs.Display_Content();
		cout << "-------------- END -------------------------------" << endl;
	}

private:
	DyMat<double> K_mat;
	DyMat<double> F_mat;
	DyMat<double> U_mat;
	DyArray<int> alldofs; 
};
FEM_parameters FEMInit(int xelm, int yelm);

#pragma endregion


#pragma region Function Assertions
DyMat<double> DyMatMul(DyMat<double> A, DyMat<double> B);
DyMat<double> DyMat_Inv(DyMat<double> A);
DyMat<double> DyMat_Add(DyMat<double> A, DyMat<double> B);
DyMat<double> DyMat_Sub(DyMat<double> A, DyMat<double> B);
DyMat<double> DyMat_dotMul(DyMat<double> A, DyMat<double> B);
DyMat<double> DyMat_dotMul(DyMat<double> A, double b);
DyMat<double> DyMat_dotDiv(DyMat<double> A, double b);
DyMat<double> DyMat_dotDiv(DyMat<double> A, DyMat<double> B);
DyMat<double> DyMat_dotPow(DyMat<double> A, double b);
DyMat<double> DyMat_transpose(DyMat<double> A);
DyMat<double> DyMat_fliplrtb(DyMat<double> A);
double DyMat_sum(DyMat<double> A);
double DyMat_max(DyMat<double> A);
void FEMupdate(FEM_parameters Data, DyMat<double> Ke, DyMat<double> x, int nelx, int nely, double penal);
DyMat<double> ChangeUpdate(DyMat<double> U, DyMat<double> Ke, DyMat<double> x, double* c, DyMat<double> dc, double penal, double rmin);
DyMat<double> Grid_filter(DyMat<double> x, DyMat<double> dc, double rmin);
DyMat<double> Optimization_Criterion(DyMat<double> x, DyMat<double> dc, double volfrac);
#pragma endregion

#endif
```