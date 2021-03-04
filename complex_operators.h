#pragma once

#include<iostream>
using namespace std;
//������
class Complex
{
public:
	//���캯��
	Complex(double r = 0.0, double i = 0.0) { re = r; im = i; }

	//���������
	friend Complex operator + (Complex& c1, Complex& c2);
	friend Complex operator + (double& d1, Complex& c2);
	friend Complex operator + (Complex& c1, double& d2);

	friend Complex operator - (Complex& c1, Complex& c2);
	friend Complex operator - (double& d1, Complex& c2);
	friend Complex operator - (Complex& c1, double& d2);

	friend Complex operator * (Complex& c1, Complex& c2);
	friend Complex operator * (double& d1, Complex& c2);
	friend Complex operator * (Complex& c1, double& d2);

	friend Complex operator / (Complex& c1, Complex& c2);
	friend Complex operator / (double& d1, Complex& c2);
	friend Complex operator / (Complex& c1, double& d2);

	//������λ��j��ʾ
	friend ostream& operator << (ostream& out, Complex& c1);
	friend istream& operator >> (istream& in, Complex& c1);

	Complex operator !();

	//���ò���
	//��ģֵ
	double modulus();
public:		//Ϊ�˵�������ʵ��
	double re, im;
};