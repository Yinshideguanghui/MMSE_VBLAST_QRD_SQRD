#pragma once

#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<fstream>
#include"complex_operators.h"
//����������
class ComplexMatrix
{
public:
	//���캯��
	explicit ComplexMatrix(int m = 1, int n = 1, bool flag = 0, Complex** incm = NULL);
	ComplexMatrix(double** a, int m, int n);
	ComplexMatrix(const ComplexMatrix& A);
	//��������
	~ComplexMatrix();
	//���
	void clear();
	//*************************************��ʼ��Ϊ�������*************************************
	//��λ��
	ComplexMatrix make_eyes(int n);

	//*************************************���������******************************************
	friend ComplexMatrix operator + (ComplexMatrix& cm1, ComplexMatrix& cm2);
	friend ComplexMatrix operator + (ComplexMatrix& cm1, double**& d1);
	friend ComplexMatrix operator + (double**& d1, ComplexMatrix& cm1);

	friend ComplexMatrix operator - (ComplexMatrix& cm1, ComplexMatrix& cm2);
	friend ComplexMatrix operator - (ComplexMatrix& cm1, double**& d1);
	friend ComplexMatrix operator - (double**& d1, ComplexMatrix& cm1);

	friend ComplexMatrix operator * (ComplexMatrix& cm1, ComplexMatrix& cm2);
	friend ComplexMatrix operator * (ComplexMatrix& cm1, double**& d1);
	friend ComplexMatrix operator * (double**& d1, ComplexMatrix& cm1);

	friend ComplexMatrix operator * (double& d1, ComplexMatrix& cm1);
	friend ComplexMatrix operator * (ComplexMatrix& cm1, double& d1);
	friend ComplexMatrix operator * (Complex& d1, ComplexMatrix& cm1);
	friend ComplexMatrix operator * (ComplexMatrix& cm1, Complex& d1);

	friend ostream & operator << (ostream& out, ComplexMatrix& cm1);
	friend istream & operator >> (istream& in, ComplexMatrix& cm1);

	ComplexMatrix operator !();
	ComplexMatrix& operator = (const ComplexMatrix A);

	//*************************************�������в���****************************************
	//����
	void exchange_row(int i1, int i2);
	//����
	void exchange_column(int j1, int j2);
	//���е�һ����Χ�ڵ���
	void exchange_some_rows_of_column(int j1, int j2, int i1, int i2);
	//�õ��У�����Ϊ�洢�±꣩
	ComplexMatrix get_row(int i);
	//�õ��У�����Ϊ�洢�±꣩
	ComplexMatrix get_column(int j);
	//�õ�����������У�����Ϊ�洢�±귶Χ��
	ComplexMatrix get_rows(int i1, int i2);
	//�õ��Ӿ��󣨲���Ϊ�洢�±귶Χ��
	ComplexMatrix get_sub_matrix(int i1, int i2, int j1, int j2);

	//�кϲ���������ͬ��ƴ�ɸ����е�һ������
	ComplexMatrix combine_columns(ComplexMatrix& A, ComplexMatrix& B);
	//�кϲ���������ͬ��ƴ�ɸ����е�һ������
	ComplexMatrix combine_rows(ComplexMatrix& A, ComplexMatrix& B);
	//ȥ��ָ����һ�У�����Ϊ�洢�±꣩
	ComplexMatrix column_delete(int j);

	//*****************************************��������****************************************
	//ǰ�������Ԫ�õ���x��ʵ����
	void forward_substitution(ComplexMatrix& A_b, ComplexMatrix& x);
	//ǰ�������Ԫ�õ���x��������
	void forward_substitution__Complex(ComplexMatrix& A_b, ComplexMatrix& x);
	//���������Ԫ�õ���x��ʵ����
	void backward_substitution(ComplexMatrix& A_b, ComplexMatrix& x);
	//���������Ԫ�õ���x��������
	void backward_substitution__Complex(ComplexMatrix& A_b, ComplexMatrix& x);

	//*****************************************����********************************************
	//�������棨������
	ComplexMatrix square_inverse();
	//��α�棨������
	ComplexMatrix Moore_Penrose_pseudo_inverse();

	//*****************************************����******************************************
	//����2-������ŷ����÷�������������
	double vector_2_norm();

	//*****************************************��˹��Ԫ****************************************
	//����ѡ��Ԫ��˹��Ԫ��ʵ����
	void Gaussian_elimination_partial_pivoting(ComplexMatrix& A_b, ComplexMatrix& x);
	//����ѡ��Ԫ��˹��Ԫ��������
	void Gaussian_elimination_partial_pivoting__Complex(ComplexMatrix& A_b, ComplexMatrix& x);

	//*****************************************QR�ֽ�*****************************************
	//��QR�ֽ�����������ʾ�ĳ������̣�ʵ����
	void solution_of_augmentedMatrix_after_QR(ComplexMatrix& Ab, ComplexMatrix& x);
	//��������Householder�任��ʵ����
	void Householder_QR_augmented(ComplexMatrix& A);
	//�������ļ���˹��ת�任��ʵ����
	void Givens_QR(ComplexMatrix& A);
	//�ŵ����ķ-ʩ������������QR�ֽ⣨ʵ����
	ComplexMatrix Gram_Schmidt_QR_classical(ComplexMatrix& Q);
	//�Ľ�����ķ-ʩ������������QR�ֽ⣨ʵ����
	ComplexMatrix Gram_Schmidt_QR_modified(ComplexMatrix& Q);
	//�Ľ�����ķ-ʩ������������QR�ֽ⣨������
	ComplexMatrix Gram_Schmidt_QR_modified__Complex(ComplexMatrix& Q);
	//��ѡ��ĸĽ�����ķ-ʩ������������QR�ֽ⣨������
	//�㷨��Դ������ϡ���ֵ���㷽�����漰������ Efficient Algorithm for Detecting Layered Space-Time Codes
	ComplexMatrix sorted_Gram_Schmidt_QR_modified__Complex(ComplexMatrix& Q, int* S);

	//*****************************************��ֵ********************************************
	//ţ�ٲ�ֵ��ö���ʽ��ʵ����
	ComplexMatrix Newton_interpolation_get_polynomial(ComplexMatrix& t, ComplexMatrix& y);
	//ţ�ٲ�ֵ����ʽ�ؾ����㷨��ֵ��ʵ����
	double Newton_interpolation_get_value(double t, ComplexMatrix& t_x, ComplexMatrix& pi);
	//ţ�ٲ�ֵ��һ����õ��µĶ���ʽ��ʵ����
	ComplexMatrix Newton_interpolation_add_one_point(double& x_new, double& y_new, ComplexMatrix& t, ComplexMatrix& pi);
	//ţ�ٲ�ֵ��ö���ʽ���ݹ鷽�������������Ϊ����±꣩��ʵ����
	ComplexMatrix Newton_interpolation_get_polynomial_recursive(ComplexMatrix& t, ComplexMatrix& y, int count);

	//*****************************************�Ż�********************************************
	//�Ż��������½�����ʵ����
	double Steepest_Descent(ComplexMatrix& x0, double(*f)(ComplexMatrix& x), double(*grad[])(ComplexMatrix& x));
	//�Ż���ţ�ٷ���ʵ����
	double Newton_unconstrained_optimization
	(ComplexMatrix& x0, double(*f)(ComplexMatrix& x), double(*grad[])(ComplexMatrix& x), double(*Hessian[])(ComplexMatrix& x));
	//�Ż�������ţ�ٷ���ʵ����
	double damped_Newton_unconstrained_optimization
	(ComplexMatrix& x0, double(*f)(ComplexMatrix& x), double(*grad[])(ComplexMatrix& x), double(*Hessian[])(ComplexMatrix& x));
	//��Լ���Ż���BFGS������������ʵ����
	double BFGS_optimization(ComplexMatrix& x0, double(*f)(ComplexMatrix& x), double(*grad[])(ComplexMatrix& x));
	//��Լ���Ż��Ĺ����ݶȷ���ʵ����beta��Fletcher-Reeves��ʽ����
	double Conjugate_Gradient_F_R(ComplexMatrix& x0, double(*f)(ComplexMatrix& x), double(*grad[])(ComplexMatrix& x));
	//��Լ���Ż��Ĺ����ݶȷ���ʵ����beta��Polak-Ribiere��ʽ����
	double Conjugate_Gradient_P_R(ComplexMatrix& x0, double(*f)(ComplexMatrix& x), double(*grad[])(ComplexMatrix& x));
	//��������С���ˣ���˹-ţ�ٷ���ʵ����
	ComplexMatrix Gauss_Newton_nonlinear_least_squares
	(ComplexMatrix& x0, const ComplexMatrix& t, const ComplexMatrix& y, 
		double(*f)(double, ComplexMatrix&), double(*Jacobian[])(double, ComplexMatrix&));
	//��ʽԼ���Ż����������ճ��ӷ�����ζ��ι滮��ʵ����
	double Lagrange_multipliers_sequential_quadratic_programming(ComplexMatrix& x0, ComplexMatrix& lambda0,
		double(*f)(ComplexMatrix& x), double(*gradf[])(ComplexMatrix& x),double(*Hf[])(ComplexMatrix& x),
		double(*g[])(ComplexMatrix& x), double(*Jg[])(ComplexMatrix& x),  double(*Hg[])(ComplexMatrix& x));
public:
	int lr;		//����
	int lc;		//����
	Complex** c;	//Ԫ��
	bool is_real;	//����Ƿ���ʵ������
};