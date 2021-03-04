#include<iostream>
#include<iomanip>
#include"complex_matrix_operators.h"
#include"complex_operators.h"
//ofstream dataOut;			//������ļ�������ʱע�͵�

//���캯��
ComplexMatrix::ComplexMatrix(int m, int n, bool flag, Complex** incm)
{
	is_real = flag;
	lr = m;		//����
	lc = n;		//����
	//���������ڴ�ռ�
	c = new Complex*[m];
	for (int i = 0; i < m; i++)
		c[i] = new Complex[n];
	//Ϊ����ֵ
	if (incm == NULL)	//û���������ʱ
	{
	
	
	}
	else				//�������ʱ
	{
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				c[i][j] = incm[i][j];
	}
}
//���캯�����Ӷ�άdouble����ת����
ComplexMatrix::ComplexMatrix(double** a, int m, int n)
{
	is_real = 1;//һ����ʵ������
	lr = m;		//����
	lc = n;		//����
	//���������ڴ�ռ�
	c = new Complex * [m];
	for (int i = 0; i < m; i++)
		c[i] = new Complex[n];
	//Ϊ����ֵ
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			c[i][j].re = a[i][j];
}
//���ƹ��캯��
ComplexMatrix::ComplexMatrix(const ComplexMatrix& A)
{
	lr = A.lr;
	lc = A.lc;
	is_real = A.is_real;
	//���������ڴ�ռ�
	c = new Complex * [lr];
	for (int i = 0; i < lr; i++)
		c[i] = new Complex[lc];
	//Ϊ����ֵ
	if (A.c == NULL)	//�������Ϊ��ʱ
		c = NULL;
	else				//�������ʱ
	{
		for (int i = 0; i < lr; i++)
			for (int j = 0; j < lc; j++)
				c[i][j] = A.c[i][j];
	}
}
//��������
ComplexMatrix::~ComplexMatrix() { clear(); }
//�������
void ComplexMatrix::clear()
{
	if (c != NULL)
	{
		for (int i = 0; i < lr; i++)
			delete[] c[i];
		delete[] c;
	}
	c = NULL;
	lr = lc = 0;
}
//��ʼ��Ϊ�������
//��λ��
ComplexMatrix ComplexMatrix::make_eyes(int n)
{
	ComplexMatrix result(n, n);
	for (int i = 0; i < n; i++) result.c[i][i].re = 1.0;
	return result;
}

//����Ӻ�+
ComplexMatrix operator + (ComplexMatrix& cm1, ComplexMatrix& cm2)
{
	if (cm1.lr != cm2.lr || cm1.lc != cm2.lc)	//ά�Ȳ�ƥ����������
	{
		cout << "ά�Ȳ�ƥ�䣬����ִ�о���ӷ�" << endl;
		system("pause");
	}
	ComplexMatrix cm3(cm1.lr, cm1.lc, (cm1.is_real&&cm2.is_real));//������Ϊʵ�������򷵻�ʵ������
	for (int i = 0; i < cm3.lr; i++)
		for (int j = 0; j < cm3.lc; j++)
			cm3.c[i][j] = cm1.c[i][j] + cm2.c[i][j];
	return cm3;
}
//����Ͷ�ά����Ӻ�+
ComplexMatrix operator + (ComplexMatrix& cm1, double**& d1)
{
	ComplexMatrix cm2(d1, cm1.lr, cm1.lc);
	return cm1 + cm2;
}
ComplexMatrix operator + (double**& d1, ComplexMatrix& cm1)
{
	ComplexMatrix cm2(d1, cm1.lr, cm1.lc);
	return cm1 + cm2;
}

//�������-
ComplexMatrix operator - (ComplexMatrix& cm1, ComplexMatrix& cm2)
{
	if (cm1.lr != cm2.lr || cm1.lc != cm2.lc)	//ά�Ȳ�ƥ����������
	{
		cout << "ά�Ȳ�ƥ�䣬����ִ�о������" << endl;
		system("pause");
	}
	ComplexMatrix cm3(cm1.lr, cm1.lc, (cm1.is_real && cm2.is_real));//������Ϊʵ�������򷵻�ʵ������
	for (int i = 0; i < cm3.lr; i++)
		for (int j = 0; j < cm3.lc; j++)
			cm3.c[i][j] = cm1.c[i][j] - cm2.c[i][j];
	return cm3;
}
//����Ͷ�ά�������-
ComplexMatrix operator - (ComplexMatrix& cm1, double**& d1)
{
	ComplexMatrix cm2(d1, cm1.lr, cm1.lc);
	return cm1 - cm2;
}
ComplexMatrix operator - (double**& d1, ComplexMatrix& cm1)
{
	ComplexMatrix cm2(d1, cm1.lr, cm1.lc);
	return cm1 - cm2;
}

//����˺�*
ComplexMatrix operator * (ComplexMatrix& cm1, ComplexMatrix& cm2)
{
	if (cm1.lc != cm2.lr)	//ά�Ȳ�ƥ����������
	{
		cout << "ά�Ȳ�ƥ�䣬����ִ�о���˷�" << endl;
		system("pause");
	}
	ComplexMatrix cm3(cm1.lr, cm2.lc, (cm1.is_real && cm2.is_real));//������Ϊʵ�������򷵻�ʵ������
	for (int i = 0; i < cm3.lr; i++)
		for (int j = 0; j < cm3.lc; j++)
			for (int k = 0; k < cm1.lc; k++)
			{
				Complex temp_c;
				temp_c = cm1.c[i][k] * cm2.c[k][j];
				cm3.c[i][j] = cm3.c[i][j] + temp_c;
			}
	return cm3;
}
//����Ͷ�ά����˺�*
ComplexMatrix operator * (ComplexMatrix& cm1, double**& d1)
{
	ComplexMatrix cm2(d1, cm1.lr, cm1.lc);
	return cm1 * cm2;
}
ComplexMatrix operator * (double**& d1, ComplexMatrix& cm1)
{
	ComplexMatrix cm2(d1, cm1.lr, cm1.lc);
	return cm1 * cm2;
}

//��������*
ComplexMatrix operator * (double& d1, ComplexMatrix& cm1)
{
	ComplexMatrix cm2(cm1.lr, cm1.lc, cm1.is_real, cm1.c);
	for (int i = 0; i < cm1.lr; i++)
		for (int j = 0; j < cm1.lc; j++)
			cm2.c[i][j] = d1 * cm2.c[i][j];
	return cm2;
}
ComplexMatrix operator * (ComplexMatrix& cm1, double& d1)
{
	ComplexMatrix cm2(cm1.lr, cm1.lc, cm1.is_real, cm1.c);
	for (int i = 0; i < cm1.lr; i++)
		for (int j = 0; j < cm1.lc; j++)
			cm2.c[i][j] = cm2.c[i][j] * d1;
	return cm2;
}
ComplexMatrix operator * (Complex& d1, ComplexMatrix& cm1)
{
	ComplexMatrix cm2(cm1.lr, cm1.lc, cm1.is_real, cm1.c);
	for (int i = 0; i < cm1.lr; i++)
		for (int j = 0; j < cm1.lc; j++)
			cm2.c[i][j] = d1 * cm2.c[i][j];
	return cm2;
}
ComplexMatrix operator * (ComplexMatrix& cm1, Complex& d1)
{
	ComplexMatrix cm2(cm1.lr, cm1.lc, cm1.is_real, cm1.c);
	for (int i = 0; i < cm1.lr; i++)
		for (int j = 0; j < cm1.lc; j++)
			cm2.c[i][j] = cm2.c[i][j] * d1;
	return cm2;
}

//�������<<
ostream & operator << (ostream& out, ComplexMatrix& cm1)
{
	if (cm1.is_real == false)		//�����������
		for (int i = 0; i < cm1.lr; i++)
		{
			for (int j = 0; j < cm1.lc; j++)
				out << cm1.c[i][j] << " ";
			out << "\n";
		}
	else if (cm1.is_real == true)	//ʵ���������
		for (int i = 0; i < cm1.lr; i++)
		{
			for (int j = 0; j < cm1.lc; j++)
				out << setw(6) << cm1.c[i][j].re << " ";
			out << "\n";
		}
	return out;
}
//��������>>
istream & operator >> (istream& in, ComplexMatrix& cm1)
{
	if (cm1.is_real == false)		//������������
		//������һ�����е�"ʵ�� �鲿",��������һ��
		for (int i = 0; i < cm1.lr; i++)
			for (int j = 0; j < cm1.lc; j++)
				in >> cm1.c[i][j];
	else if (cm1.is_real == true)	//ʵ����������
		for (int i = 0; i < cm1.lr; i++)
			for (int j = 0; j < cm1.lc; j++)
				in >> cm1.c[i][j].re;
	return in;
}

//������ת��!
ComplexMatrix ComplexMatrix::operator !()
{
	ComplexMatrix cm1(this->lc, this->lr, this->is_real);
	for (int i = 0; i < cm1.lr; i++)
		for (int j = 0; j < cm1.lc; j++)
			cm1.c[i][j] = !((this->c)[j][i]);
	return cm1;
}

//���ظ�ֵ�����
ComplexMatrix& ComplexMatrix::operator = (const ComplexMatrix A)
{
	if (this->c != A.c)
	{
		this->clear();		//�������ǰ�Ⱥ���߶��ͷ��ڴ�ǳ���Ҫ
		this->lr = A.lr;
		this->lc = A.lc;
		this->is_real = A.is_real;
		//���������ڴ�ռ�
		this->c = new Complex * [A.lr];
		for (int i = 0; i < A.lr; i++)
			c[i] = new Complex[A.lc];
		//Ϊ����ֵ
		if (A.c == NULL)	//�������Ϊ��ʱ
			this->c = NULL;
		else				//�������ʱ
		{
			for (int i = 0; i < A.lr; i++)
				for (int j = 0; j < A.lc; j++)
					this->c[i][j] = A.c[i][j];
		}
	}
	return *this;
}

//����
void ComplexMatrix::exchange_row(int i1, int i2)
{
	Complex temp;
	for (int j = 0; j < lc; j++)
	{
		temp = c[i1][j];
		c[i1][j] = c[i2][j];
		c[i2][j] = temp;
	}
}
//����
void ComplexMatrix::exchange_column(int j1, int j2)
{	//����
	Complex temp;
	for (int i = 0; i < lr; i++)
	{
		temp = c[i][j1];
		c[i][j1] = c[i][j2];
		c[i][j2] = temp;
	}
}
//���е�һ����Χ�ڵ���
void ComplexMatrix::exchange_some_rows_of_column(int j1, int j2, int i1, int i2)
{
	Complex temp;
	for (int i = i1; i <= i2; i++)
	{
		temp = c[i][j1];
		c[i][j1] = c[i][j2];
		c[i][j2] = temp;
	}
}
//�õ��У�����Ϊ�洢�±꣩
ComplexMatrix ComplexMatrix::get_row(int i)
{
	ComplexMatrix target_row(1, lc, is_real);
	for (int k = 0; k < lc; k++) target_row.c[0][k] = c[i][k];
	return target_row;
}
//�õ��У�����Ϊ�洢�±꣩
ComplexMatrix ComplexMatrix::get_column(int j)
{
	ComplexMatrix target_column(lr, 1, is_real);
	for (int k = 0; k < lr; k++) target_column.c[k][0] = c[k][j];
	return target_column;
}
//�õ�����������У�����Ϊ�洢�±귶Χ��
ComplexMatrix ComplexMatrix::get_rows(int i1, int i2)
{
	//if (i1 > i2)  NULL
	ComplexMatrix target_rows(i2 - i1 + 1, lc, is_real);
	for (int i = i1; i <= i2; i++)
		for (int j = 0; j < lc; j++)
			target_rows.c[i - i1][j] = c[i][j];
	return target_rows;
}
//�õ��Ӿ��󣨲���Ϊ�洢�±귶Χ��
ComplexMatrix ComplexMatrix::get_sub_matrix(int i1, int i2, int j1, int j2)
{
	//if (i1 > i2 || j1 > j2)  NULL
	ComplexMatrix target_sub_matrix(i2 - i1 + 1, j2 - j1 + 1, is_real);
	for (int i = i1; i <= i2; i++)
		for (int j = j1; j <= j2; j++)
			target_sub_matrix.c[i - i1][j - j1] = c[i][j];
	return target_sub_matrix;
}

//�кϲ���������ͬ��ƴ�ɸ����е�һ������
ComplexMatrix ComplexMatrix::combine_columns(ComplexMatrix& A, ComplexMatrix& B)
{
	ComplexMatrix result(A.lr, A.lc + B.lc, (A.is_real && B.is_real));
	for (int i = 0; i < A.lr; i++)
	{
		for (int j = 0; j < A.lc; j++)result.c[i][j] = A.c[i][j];
		for (int j = A.lc; j < result.lc; j++)result.c[i][j] = B.c[i][j - A.lc];
	}
	return result;
}
//�кϲ���������ͬ��ƴ�ɸ����е�һ������
ComplexMatrix ComplexMatrix::combine_rows(ComplexMatrix& A, ComplexMatrix& B)
{
	ComplexMatrix result(A.lr + B.lr, A.lc, (A.is_real && B.is_real));
	for (int j = 0; j < A.lc; j++)
	{
		for (int i = 0; i < A.lr; i++)result.c[i][j] = A.c[i][j];
		for (int i = A.lr; i < result.lr; i++)result.c[i][j] = B.c[i - A.lr][j];
	}
	return result;
}
//ȥ��ָ����һ�У�����Ϊ�洢�±꣩
ComplexMatrix ComplexMatrix::column_delete(int k)
{
	ComplexMatrix result(lr, lc - 1, is_real);
	for (int i = 0; i < lr; i++)
	{
		for (int j = 0; j < k; j++)result.c[i][j] = c[i][j];
		for (int j = k + 1; j < lc; j++)result.c[i][j - 1] = c[i][j];
	}
	return result;
}

//ǰ�������Ԫ�õ���x��ʵ����
void ComplexMatrix::forward_substitution(ComplexMatrix& A_b, ComplexMatrix& x)
{
	for (int i = 0; i < A_b.lr; i++)
	{
		for (int j = 0; j < i; j++)
			A_b.c[i][A_b.lc - 1].re -= (A_b.c[i][j].re * x.c[j][0].re);
		x.c[i][0].re = A_b.c[i][A_b.lc - 1].re / A_b.c[i][i].re;
	}
}
//ǰ�������Ԫ�õ���x��������
void ComplexMatrix::forward_substitution__Complex(ComplexMatrix& A_b, ComplexMatrix& x)
{
	Complex temp;
	for (int i = 0; i < A_b.lr; i++)
	{
		for (int j = 0; j < i; j++)
		{
			temp = A_b.c[i][j] * x.c[j][0];
			A_b.c[i][A_b.lc - 1] = A_b.c[i][A_b.lc - 1] - temp;
		}
		x.c[i][0] = A_b.c[i][A_b.lc - 1] / A_b.c[i][i];
	}
}
//���������Ԫ�õ���x��ʵ����
void ComplexMatrix::backward_substitution(ComplexMatrix& A_b, ComplexMatrix& x)
{
	for (int i = A_b.lr - 1; i >= 0; i--)
	{
		for (int j = A_b.lr - 1; j > i; j--)
			A_b.c[i][A_b.lc - 1].re -= (A_b.c[i][j].re * x.c[j][0].re);
		x.c[i][0].re = A_b.c[i][A_b.lc - 1].re / A_b.c[i][i].re;
	}
}
//���������Ԫ�õ���x��������
void ComplexMatrix::backward_substitution__Complex(ComplexMatrix& A_b, ComplexMatrix& x)
{
	Complex temp;
	for (int i = A_b.lr - 1; i >= 0; i--)
	{
		for (int j = A_b.lr - 1; j > i; j--)
		{
			temp = A_b.c[i][j] * x.c[j][0];
			A_b.c[i][A_b.lc - 1] = A_b.c[i][A_b.lc - 1] - temp;
		}
		x.c[i][0] = A_b.c[i][A_b.lc - 1] / A_b.c[i][i];
	}
}

//�������棨������
ComplexMatrix ComplexMatrix::square_inverse()
{
	//���̷߳�����Ax=b�ķ�ʽ��b�ֱ�ȡ��λ��ĸ�������������x�������Ķ�Ӧ��������ƴ������󼴿�
	//if (lr != lc)return NULL;
	ComplexMatrix A(lr, lc, is_real, c);		//��ǰ����
	ComplexMatrix b(lr, 1, is_real);			//����ÿ�ε�b����
	ComplexMatrix Ab(lr, lc + 1, is_real);		//����ÿ�ε��������
	ComplexMatrix x(lr, 1, is_real);			//����ÿ�εĽ�����
	ComplexMatrix result(lr, lc, is_real);		//�������
	int ii = 0;									//�����Ż���������
	ComplexMatrix r;							//�в�����
	ComplexMatrix z(lr, 1, is_real);			//����������
	for (int i = 0; i < lc; i++)	//���ѭ����������������ͬ
	{
		//b����Ϊ��i��Ԫ��Ϊ1��������
		for (int k = 0; k < b.lr; k++)
		{
			b.c[k][0].re = 0.0;
			b.c[k][0].im = 0.0;
		}
		b.c[i][0].re = 1.0;
		//A��bƴ��������󣬲��ɲ���ѡ��Ԫ��˹��x
		Ab = combine_columns(A, b);
		Gaussian_elimination_partial_pivoting__Complex(Ab, x);
		ii = 0;
		//�����Ż�10��
		do {
			r = A * x; r = b - r;
			Ab = Ab.column_delete(Ab.lc - 1);
			Ab = Ab.combine_columns(Ab, r);
			backward_substitution__Complex(Ab, z);
			x = x + z;
			ii++;
		} while (ii < 10);
		//�����x�ǽ������ĵ�i��
		for (int k = 0; k < x.lr; k++) result.c[k][i] = x.c[k][0];
	}
	//�ͷ��ڴ�
	//A.clear(); b.clear(); Ab.clear(); x.clear(); r.clear(); z.clear();
	return result;
}
//��α�棨������
ComplexMatrix ComplexMatrix::Moore_Penrose_pseudo_inverse()
{
	ComplexMatrix A(lr, lc, is_real, c);
	ComplexMatrix A_H = !A;
	ComplexMatrix temp = A_H * A;
	temp = temp.square_inverse();
	temp = temp * A_H;
	return temp;
}

//����2-������ŷ����÷�������������
double ComplexMatrix::vector_2_norm()
{
	double result = 0.0;
	if (lr == 1)		//������
	{
		for (int j = 0; j < lc; j++)
			result = result + c[0][j].re * c[0][j].re + c[0][j].im * c[0][j].im;
		result = sqrt(result);
		return result;
	}
	else if (lc == 1)	//������
	{
		for (int i = 0; i < lr; i++)
			result = result + c[i][0].re * c[i][0].re + c[i][0].im * c[i][0].im;
		result = sqrt(result);
		return result;
	}
	else return 0.0;	//�����ǣ�����������ʹ�������Ҫ���֣�
}

//����ѡ��Ԫ��˹��Ԫ��ʵ����
void ComplexMatrix::Gaussian_elimination_partial_pivoting(ComplexMatrix& A_b, ComplexMatrix& x)
{
	//��ϵ������Ϊ��������
	for (int i = 0; i < A_b.lr; i++)
	{
		//ѡ��������Ԫ
		double temp_max = A_b.c[i][i].re;
		int temp_max_row = i;
		for (int j = i + 1; j < A_b.lr; j++)
			if (abs(A_b.c[j][i].re) > abs(temp_max))
			{
				temp_max = A_b.c[j][i].re;
				temp_max_row = j;
			}
		A_b.exchange_row(i, temp_max_row);

		//����Ԫ��Ԫ
		for (int k = i + 1; k < A_b.lr; k++)
		{
			double temp = -A_b.c[k][i].re / A_b.c[i][i].re;
			for (int j = i; j < A_b.lc; j++)
				A_b.c[k][j].re = A_b.c[k][j].re + temp * A_b.c[i][j].re;
		}
	}
	//���������Ԫ�õ���x
	backward_substitution(A_b, x);
}
//����ѡ��Ԫ��˹��Ԫ��������
void ComplexMatrix::Gaussian_elimination_partial_pivoting__Complex(ComplexMatrix& A_b, ComplexMatrix& x)
{
	//��ϵ������Ϊ��������
	for (int i = 0; i < A_b.lr; i++)
	{
		//ѡ��������Ԫ
		Complex temp_max = A_b.c[i][i];
		int temp_max_row = i;
		for (int j = i + 1; j < A_b.lr; j++)
			if (A_b.c[j][i].modulus() > temp_max.modulus())
			{
				temp_max = A_b.c[j][i];
				temp_max_row = j;
			}
		A_b.exchange_row(i, temp_max_row);

		//����Ԫ��Ԫ
		for (int k = i + 1; k < A_b.lr; k++)
		{
			Complex temp = A_b.c[k][i] / A_b.c[i][i];
			Complex temptemp;
			for (int j = i; j < A_b.lc; j++)
			{
				temptemp = temp * A_b.c[i][j];
				A_b.c[k][j] = A_b.c[k][j] - temptemp;
			}
		}
	}
	//���������Ԫ�õ���x
	backward_substitution__Complex(A_b, x);
}


//��QR�ֽ�����������ʾ�ĳ������̣�ʵ����
void ComplexMatrix::solution_of_augmentedMatrix_after_QR(ComplexMatrix& Ab, ComplexMatrix& x)
{
	//��������ϵͳ����С���˽�
	ComplexMatrix Rb(x.lr, x.lr + 1, true);
	for (int i = 0; i < Rb.lr; i++)
		for (int j = 0; j < Rb.lc; j++)
			Rb.c[i][j].re = Ab.c[i][j].re;
	backward_substitution(Rb, x);
}
//��������Householder�任��ʵ����
void ComplexMatrix::Householder_QR_augmented(ComplexMatrix& A)
{
	ComplexMatrix v_k(A.lr, 1, true);		//�����˹����������
	ComplexMatrix v_k_T(1, A.lr, true);		//�����˹������������ת��
	ComplexMatrix v_k_j(A.lr, 1, true);		//�����������ĺ�˹����������
	ComplexMatrix vkT_vk(1, 1, true);		//�����˹�����������������ڻ�
	double beta_k = 0.0;					//�����˹�����������������ڻ�
	ComplexMatrix A_j(A.lr, 1, true);		//����A������ȡ��������
	ComplexMatrix gamma_j_CM(1, 1, true);	//�����ʣ���Ӿ������任ʱ��ϵ��
	double gamma_j = 0.0;					//�����ʣ���Ӿ������任ʱ��ϵ��
	for (int k = 0; k < A.lc - 1; k++)	//for�������
	{
		//���㵱ǰ�еĺ�˹����������
		double square_sum = 0.0;
		for (int i = k; i < A.lr; i++)square_sum += pow(A.c[i][k].re, 2);
		double alpha_k = ((A.c[k][k].re >= 0) ? (-1.0) : 1.0) * sqrt(square_sum);
		for (int i = 0; i < k; i++)v_k.c[i][0].re = 0.0;
		for (int i = k; i < A.lr; i++)v_k.c[i][0].re = A.c[i][k].re;
		v_k.c[k][0].re = v_k.c[k][0].re - alpha_k;
		v_k_T = !v_k;
		vkT_vk = v_k_T * v_k;
		beta_k = vkT_vk.c[0][0].re;
		//�����ǰ���Ѿ�Ϊ�㣬����
		if (abs(beta_k) < 1e-323) continue;
		//��ʣ����Ӿ������任
		for (int j = k; j < A.lc; j++)
		{
			for (int i = 0; i < A.lr; i++)A_j.c[i][0].re = A.c[i][j].re;
			gamma_j_CM = v_k_T * A_j;
			gamma_j = gamma_j_CM.c[0][0].re;
			gamma_j = 2.0 * gamma_j / beta_k;
			for (int i = 0; i < A.lr; i++)v_k_j.c[i][0].re = v_k.c[i][0].re;
			v_k_j = gamma_j * v_k;
			A_j = A_j - v_k_j;
			for (int i = 0; i < A.lr; i++)A.c[i][j].re = A_j.c[i][0].re;
		}
	}
}
//�������ļ���˹��ת�任��ʵ����
void ComplexMatrix::Givens_QR(ComplexMatrix& A)
{	//��ϵ���������Խ�����Ԫ��ѭ��
	for (int i = 0; i < A.lc - 1; i++)
		for (int j = i + 1; j < A.lr; j++)
			if (abs(A.c[j][i].re) >= 1e-323)	//����Ԫ�ز�������
			{
				//���켪��˹��ת����
				ComplexMatrix Givens_rotation(A.lr, A.lr, true);
				for (int k = 0; k < Givens_rotation.lr; k++)Givens_rotation.c[k][k].re = 1.0;
				double a1 = A.c[i][i].re;	//�������������Խ�Ԫ�����ת����ֹǰһ���ֳ��ַ���Ԫ
				double a2 = A.c[j][i].re;
				double c = a1 / sqrt(pow(a1, 2) + pow(a2, 2));
				double s = a2 / sqrt(pow(a1, 2) + pow(a2, 2));
				Givens_rotation.c[i][i].re = c;
				Givens_rotation.c[i][j].re = s;
				Givens_rotation.c[j][i].re = -s;
				Givens_rotation.c[j][j].re = c;
				//����˹��ת
				A = Givens_rotation * A;
			}
}
//�ŵ����ķ-ʩ������������QR�ֽ⣨ʵ����
ComplexMatrix ComplexMatrix::Gram_Schmidt_QR_classical(ComplexMatrix& Q)
{
	//Q�����ֽ⣬ԭʼ���ݴ���A
	ComplexMatrix A(Q.lr, Q.lc, true);
	for (int i = 0; i < Q.lr; i++)for (int j = 0; j < Q.lc; j++)A.c[i][j].re = Q.c[i][j].re;
	//QR�ֽ��R
	ComplexMatrix R(Q.lc, Q.lc, true);

	ComplexMatrix qk(A.lr, 1, true);	//���в���
	ComplexMatrix qkT(1, A.lr, true);	//���в���
	ComplexMatrix qj(A.lr, 1, true);	//���в���
	ComplexMatrix qjT(1, A.lr, true);	//���в���
	ComplexMatrix ak(A.lr, 1, true);	//��ȡ��

	ComplexMatrix rjk(1, 1, true);		//�ڻ���ʱ����
	double r_jk = 0.0;					//�ڻ���ʱ����
	ComplexMatrix rkk(1, 1, true);		//��������ʱ����
	double r_kk = 0.0;					//��������ʱ����
	for (int k = 0; k < A.lc; k++)	//����ѭ��
	{
		for (int i = 0; i < A.lr; i++)
		{
			qk.c[i][0].re = A.c[i][k].re;
			ak.c[i][0].re = A.c[i][k].re;
		}
		//�ӵ�ǰ���м�ȥ����ǰ�����еķ���
		for (int j = 0; j < k; j++)
		{
			for (int i = 0; i < Q.lr; i++)
				qj.c[i][0].re = Q.c[i][j].re;
			qjT = !qj;
			rjk = qjT * ak;
			r_jk = rjk.c[0][0].re;
			R.c[j][k].re = r_jk;
			qj = r_jk * qj;
			qk = qk - qj;
		}
		qkT = !qk;
		rkk = qkT * qk;
		r_kk = sqrt(rkk.c[0][0].re);
		R.c[k][k].re = r_kk;
		//���������أ����ж�
		if (abs(r_kk) < 1e-323)break;
		//����ǰ�б�׼��
		r_kk = 1 / r_kk;
		qk = r_kk * qk;
		for (int i = 0; i < Q.lr; i++)Q.c[i][k].re = qk.c[i][0].re;
	}
	return R;
}
//�Ľ�����ķ-ʩ������������QR�ֽ⣨ʵ����
ComplexMatrix ComplexMatrix::Gram_Schmidt_QR_modified(ComplexMatrix& Q)
{
	//Q�����ֽ⣬ԭʼ���ݴ���A
	ComplexMatrix A(Q.lr, Q.lc, true);
	for (int i = 0; i < Q.lr; i++)for (int j = 0; j < Q.lc; j++)A.c[i][j].re = Q.c[i][j].re;
	//QR�ֽ��R
	ComplexMatrix R(Q.lc, Q.lc, true);

	ComplexMatrix ak(A.lr, 1, true);		//��ȡ��
	ComplexMatrix akT(1, A.lr, true);		//��ת��
	ComplexMatrix aj(A.lr, 1, true);		//���в���
	ComplexMatrix qk(A.lr, 1, true);		//���в���
	ComplexMatrix r_kj_qk(A.lr, 1, true);	//���в���
	ComplexMatrix qkT(1, A.lr, true);		//���в���

	ComplexMatrix rkj(1, 1, true);		//�ڻ���ʱ����
	double r_kj = 0.0;					//�ڻ���ʱ����
	ComplexMatrix rkk(1, 1, true);		//��������ʱ����
	double r_kk = 0.0;					//��������ʱ����

	for (int k = 0; k < A.lc; k++)	//����ѭ��
	{
		for (int i = 0; i < A.lr; i++)
			ak.c[i][0].re = A.c[i][k].re;
		akT = !ak;
		rkk = akT * ak;
		r_kk = sqrt(rkk.c[0][0].re);
		R.c[k][k] = r_kk;
		//���������أ����ж�
		if (abs(r_kk) < 1e-323)break;
		//����ǰ�б�׼��
		r_kk = 1 / r_kk;
		qk = r_kk * ak;
		qkT = !qk;
		for (int i = 0; i < Q.lr; i++)Q.c[i][k].re = qk.c[i][0].re;
		//��ȥ�������ڵ�ǰ���ϵķ���
		for (int j = k + 1; j < A.lc; j++)
		{
			for (int i = 0; i < A.lr; i++) aj.c[i][0].re = A.c[i][j].re;
			rkj = qkT * aj;
			R.c[k][j] = r_kj = rkj.c[0][0].re;
			r_kj_qk = r_kj * qk;
			aj = aj - r_kj_qk;
			for (int i = 0; i < A.lr; i++) A.c[i][j].re = aj.c[i][0].re;
		}
	}
	return R;
}
//�Ľ�����ķ-ʩ������������QR�ֽ⣨������
ComplexMatrix ComplexMatrix::Gram_Schmidt_QR_modified__Complex(ComplexMatrix& Q)
{
	//QR�ֽ��R
	ComplexMatrix R(Q.lc, Q.lc, Q.is_real);

	ComplexMatrix ak(Q.lr, 1);		//��ȡ��
	ComplexMatrix aj(Q.lr, 1);		//���в���
	ComplexMatrix qk(Q.lr, 1);		//���в���
	ComplexMatrix r_kj_qk(Q.lr, 1);	//���в���
	ComplexMatrix qkH(1, Q.lr);		//���в���

	ComplexMatrix r_kj(1, 1);		//�ڻ���ʱ����
	double r_kk = 0.0;				//��������ʱ����

	for (int k = 0; k < Q.lc; k++)	//����ѭ��
	{
		ak = Q.get_column(k);
		r_kk = ak.vector_2_norm();
		R.c[k][k].re = r_kk;
		//���������أ����ж�
		if (abs(r_kk) < 1e-323)break;
		//����ǰ�б�׼��
		r_kk = 1 / r_kk;
		qk = r_kk * ak;
		qkH = !qk;
		for (int i = 0; i < Q.lr; i++)Q.c[i][k] = qk.c[i][0];
		//��ȥ�������ڵ�ǰ���ϵķ���
		for (int j = k + 1; j < Q.lc; j++)
		{
			aj = Q.get_column(j);
			r_kj = qkH * aj;
			R.c[k][j] = r_kj.c[0][0];
			r_kj_qk = r_kj.c[0][0] * qk;
			aj = aj - r_kj_qk;
			for (int i = 0; i < Q.lr; i++) Q.c[i][j] = aj.c[i][0];
		}
	}
	//�ͷ��ڴ�
	//ak.clear(); aj.clear(); qk.clear(); r_kj_qk.clear(); qkH.clear(); r_kj.clear();
	return R;
}
//��ѡ��ĸĽ�����ķ-ʩ������������QR�ֽ⣨������
//�㷨��Դ������ϡ���ֵ���㷽�����漰������ Efficient Algorithm for Detecting Layered Space-Time Codes
ComplexMatrix ComplexMatrix::sorted_Gram_Schmidt_QR_modified__Complex(ComplexMatrix& Q, int* S)
{
	//QR�ֽ��R
	ComplexMatrix R(Q.lc, Q.lc, Q.is_real);

	ComplexMatrix ai(Q.lr, 1);		//��ȡ��
	ComplexMatrix aj(Q.lr, 1);		//���в���
	ComplexMatrix qi(Q.lr, 1);		//���в���
	ComplexMatrix r_ij_qi(Q.lr, 1);	//���в���
	ComplexMatrix qiH(1, Q.lr);		//���в���

	ComplexMatrix q;				//�洢Q����
	double q_2_norm;				//�洢q��2-����
	double q_2_norm_min;			//�洢Q��2-������С��һ�е�2-����
	int q_2_norm_min_column;		//�洢Q��2-������С��һ�еĴ洢�±�
	int temp;						//�����н�

	ComplexMatrix r_ij(1, 1);		//�ڻ���ʱ����
	double r_ii = 0.0;				//��������ʱ����

	for (int i = 0; i < Q.lc; i++)	//����ѭ��
	{
		//�ҳ�Qʣ������2-������С��һ��
		q_2_norm_min_column = i;
		q = Q.get_column(i);
		q_2_norm_min = q.vector_2_norm();
		for (int k = i + 1; k < Q.lc; k++)
		{
			q = Q.get_column(k);
			q_2_norm = q.vector_2_norm();
			if (q_2_norm < q_2_norm_min)
			{
				q_2_norm_min_column = k;
				q_2_norm_min = q_2_norm;
			}
		}
		//����Q,R,S�и������2-������С�ж�Ӧ���к͵�ǰ�ĵ�i��
		Q.exchange_column(i, q_2_norm_min_column);
		R.exchange_column(i, q_2_norm_min_column);
		temp = S[i];
		S[i] = S[q_2_norm_min_column];
		S[q_2_norm_min_column] = temp;
		//��ʼ������
		ai = Q.get_column(i);
		r_ii = ai.vector_2_norm();
		R.c[i][i].re = r_ii;
		//���������أ����ж�
		if (abs(r_ii) < 1e-323)break;
		//����ǰ�б�׼��
		r_ii = 1 / r_ii;
		qi = r_ii * ai;
		qiH = !qi;
		for (int k = 0; k < Q.lr; k++)Q.c[k][i] = qi.c[k][0];
		//��ȥ�������ڵ�ǰ���ϵķ���
		for (int j = i + 1; j < Q.lc; j++)
		{
			aj = Q.get_column(j);
			r_ij = qiH * aj;
			R.c[i][j] = r_ij.c[0][0];
			r_ij_qi = r_ij.c[0][0] * qi;
			aj = aj - r_ij_qi;
			for (int k = 0; k < Q.lr; k++) Q.c[k][j] = aj.c[k][0];
		}
	}
	//�ͷ��ڴ�
	//ai.clear(); aj.clear(); qi.clear(); r_ij_qi.clear(); qiH.clear(); q.clear(); r_ij.clear();
	return R;
}

//ţ�ٲ�ֵ��ö���ʽ��ʵ����
ComplexMatrix ComplexMatrix::Newton_interpolation_get_polynomial(ComplexMatrix& t, ComplexMatrix& y)
{
	ComplexMatrix pi_(t.lr, 1, true);		//��ֵ����ʽϵ��
	ComplexMatrix Ab(t.lr, t.lr + 1, true);	//�������
	//���������ֵ
	for (int i = 0; i < Ab.lr; i++)
	{
		Ab.c[i][Ab.lc - 1].re = y.c[i][0].re;	//��ʽ�ұߵ�y
		for (int j = Ab.lc - 2; j > i; j--)Ab.c[i][j].re = 0.0;	//�ϸ������ǵ���
		for (int j = 0; j <= i; j++)	//�����ǵ�ϵ��
		{
			Ab.c[i][j].re = 1.0;
			for (int k = 0; k < j; k++)
				Ab.c[i][j].re *= (t.c[i][0].re - t.c[k][0].re);
		}
	}
	//���ֵ����ʽϵ��
	forward_substitution(Ab, pi_);
	return pi_;
}
//ţ�ٲ�ֵ����ʽ�ؾ����㷨��ֵ��ʵ����
double ComplexMatrix::Newton_interpolation_get_value(double t, ComplexMatrix& t_x, ComplexMatrix& pi_)
{
	double p = pi_.c[pi_.lr-1][0].re;
	for (int i = pi_.lr - 2; i >= 0; i--)
	{
		p *= (t - t_x.c[i][0].re);
		p += pi_.c[i][0].re;
	}
	return p;
}
//ţ�ٲ�ֵ��һ����õ��µĶ���ʽ��ʵ����
ComplexMatrix ComplexMatrix::Newton_interpolation_add_one_point(double& x_new, double& y_new, ComplexMatrix& t, ComplexMatrix& pi_)
{
	//t���ϵ��Ա�����pi_���ϵĲ�ֵ����ʽϵ��
	ComplexMatrix pi_new(pi_.lr + 1, 1, true);
	for (int i = 0; i < pi_.lr; i++)pi_new.c[i][0].re = pi_.c[i][0].re;
	ComplexMatrix new_equation(1, pi_.lr + 1, true);	//�¼�һ���㵼�����ֵϵ���¼�һ�����̣�֮ǰ��ϵ������
	//�·��̵����Ա���������ϵ��
	for (int j = 0; j < pi_.lr; j++)
	{
		new_equation.c[0][j].re = 1.0;
		for (int k = 0; k < j; k++)
			new_equation.c[0][j].re *= (x_new - t.c[k][0].re);
	}
	//�·��̵����һ��ϵ��
	new_equation.c[0][pi_.lr].re = 1.0;
	for (int k = 0; k < pi_.lr; k++)
		new_equation.c[0][pi_.lr].re *= (x_new - t.c[k][0].re);
	//������µ�ϵ��
	double pn = y_new;
	for (int i = 0; i < pi_.lr; i++)
		pn -= (new_equation.c[0][i].re * pi_.c[i][0].re);
	pn /= new_equation.c[0][pi_.lr].re;
	//���ؽ��
	pi_new.c[pi_new.lr - 1][0].re = pn;
	return pi_new;
}
//ţ�ٲ�ֵ��ö���ʽ���ݹ鷽�������������Ϊ����±꣩��ʵ����
ComplexMatrix ComplexMatrix::Newton_interpolation_get_polynomial_recursive(ComplexMatrix& t, ComplexMatrix& y, int count)
{
	if (count == 0)
	{
		ComplexMatrix pi_(1, 1, true);
		pi_.c[0][0].re = y.c[0][0].re;
		return pi_;
	}
	else
	{
		ComplexMatrix pi_0 = Newton_interpolation_get_polynomial_recursive(t, y, count - 1);
		ComplexMatrix pi_ = Newton_interpolation_add_one_point(t.c[count][0].re, y.c[count][0].re, t, pi_0);
		return pi_;
	}
}

//�Ż��������½�����ʵ����
double ComplexMatrix::Steepest_Descent(ComplexMatrix& x0, double(*f)(ComplexMatrix& x), double(*grad[])(ComplexMatrix& x))
{
	ComplexMatrix xk(x0.lr, x0.lc, x0.is_real, x0.c);	//���������
	ComplexMatrix sk(xk.lr, xk.lc, xk.is_real);			//���渺�ݶ�
	ComplexMatrix sk0;									//���������õĸ��ݶ�
	double alpha0;										//������������һ�β���
	double alpha;										//������������
	double fx0;											//����������������һ�εĺ���ֵ
	double fx;											//�������������б��εĺ���ֵ
	//dataOut.open("Steepest_Descent.txt");							//������ļ�������ʱע�͵�
	//dataOut << xk.c[0][0].re << " " << xk.c[1][0].re << endl;		//���x���ļ�������ʱע�͵�
	//���ʼ���ݶ�
	for (int i = 0; i < xk.lr; i++)
		sk.c[i][0].re = -((grad[i])(xk));
	do {
		//���������󲽳���alphaÿ������0.001��					//���ǵ�һ��ֱ����sk��֮��ÿ��/2����������
		ComplexMatrix sk_temp(sk.lr, sk.lc, sk.is_real, sk.c);	//����������������ʱ��ʱs
		ComplexMatrix xk_temp(xk.lr, xk.lc, xk.is_real, xk.c);	//����������������ʱ��ʱx
		alpha = 0.0;	//��ʼ������
		do {
			alpha0 = alpha;
			fx0 = f(xk_temp);
			alpha += 0.001;
			sk_temp = alpha * sk;
			xk_temp = xk_temp + sk_temp;
			fx = f(xk_temp);
		} while (fx < fx0);	//�ѳ�ʱfx >= fx0��fx0��Ӧ��alpha0Ϊ�����ҵ���Сֵ�Ĳ���
		//������
		sk = alpha0 * sk;
		sk0 = sk;
		xk = xk + sk;		//cout << xk << endl;		//�������·��������ʱע�͵�
		//���¸��ݶ�
		for (int i = 0; i < xk.lr; i++)
			sk.c[i][0].re = -((grad[i])(xk));
	//	dataOut << xk.c[0][0].re << " " << xk.c[1][0].re << endl;		//���x���ļ�������ʱע�͵�
	} while (sk0.vector_2_norm() >= 1e-15);	//���ݶ�Ϊ��ʱ�ѳ�								//��������try�Ƴ��Դ���
	//dataOut.close();													//������ļ�������ʱע�͵�
	cout << xk << endl;													//�������x������ʱע�͵�
	return f(xk);
}
//�Ż���ţ�ٷ���ʵ����
double ComplexMatrix::Newton_unconstrained_optimization
(ComplexMatrix& x0, double(*f)(ComplexMatrix& x), double(*grad[])(ComplexMatrix& x), double(*Hessian[])(ComplexMatrix& x))
{
	ComplexMatrix xk(x0.lr, x0.lc, x0.is_real, x0.c);	//�������������
	ComplexMatrix gradk(xk.lr, xk.lc, xk.is_real);		//���渺�ݶ�����
	ComplexMatrix Hk(xk.lr, xk.lr, xk.is_real);			//�����������
	ComplexMatrix Hk_gradk;								//�����������
	ComplexMatrix sk(xk.lr, xk.lc, xk.is_real);			//����ţ�ٲ�������
	//dataOut.open("Newton_optimization.txt");						//������ļ�������ʱע�͵�
	//dataOut << xk.c[0][0].re << " " << xk.c[1][0].re << endl;		//���x���ļ�������ʱע�͵�
	//���ʼ���ݶ�
	for (int i = 0; i < xk.lr; i++)
		gradk.c[i][0].re = -((grad[i])(xk));
	do {
		//��ţ�ٲ���
			//���������
		for (int i = 0; i < xk.lr; i++)
			for (int j = 0; j < xk.lr; j++)
				Hk.c[i][j].re = (Hessian[i * x0.lr + j])(xk);
			//������������ò���ѡ��Ԫ��˹��Ԫ�ⷽ����
		Hk_gradk = Hk.combine_columns(Hk, gradk);
		Gaussian_elimination_partial_pivoting(Hk_gradk, sk);
		//������
		xk = xk + sk;						//cout << xk << endl;		//�������·��������ʱע�͵�
		//���¸��ݶ�
		for (int i = 0; i < xk.lr; i++)
			gradk.c[i][0].re = -((grad[i])(xk));
	//	dataOut << xk.c[0][0].re << " " << xk.c[1][0].re << endl;		//���x���ļ�������ʱע�͵�
	} while (gradk.vector_2_norm() >= 1e-15);	//���ݶ�Ϊ��ʱ�ѳ�								//��������try�Ƴ��Դ���
	//dataOut.close();													//������ļ�������ʱע�͵�
	cout << xk << endl;													//�������x������ʱע�͵�
	return f(xk);
}
//�Ż�������ţ�ٷ���ʵ����
double ComplexMatrix::damped_Newton_unconstrained_optimization
(ComplexMatrix& x0, double(*f)(ComplexMatrix& x), double(*grad[])(ComplexMatrix& x), double(*Hessian[])(ComplexMatrix& x))
{
	ComplexMatrix xk(x0.lr, x0.lc, x0.is_real, x0.c);	//�������������
	ComplexMatrix gradk(xk.lr, xk.lc, xk.is_real);		//���渺�ݶ�����
	ComplexMatrix Hk(xk.lr, xk.lr, xk.is_real);			//�����������
	ComplexMatrix Hk_gradk;								//�����������
	ComplexMatrix sk(xk.lr, xk.lc, xk.is_real);			//����ţ�ٲ�������
	double alpha0;										//������������һ�β���
	double alpha;										//������������
	double fx0;											//����������������һ�εĺ���ֵ
	double fx;											//�������������б��εĺ���ֵ
	//dataOut.open("Damped_Newton.txt");								//������ļ�������ʱע�͵�
	//dataOut << xk.c[0][0].re << " " << xk.c[1][0].re << endl;			//���x���ļ�������ʱע�͵�
	//���ʼ���ݶ�
	for (int i = 0; i < xk.lr; i++)
		gradk.c[i][0].re = -((grad[i])(xk));
	do {
		//��ţ�ٲ���
			//���������
		for (int i = 0; i < xk.lr; i++)
			for (int j = 0; j < xk.lr; j++)
				Hk.c[i][j].re = (Hessian[i * x0.lr + j])(xk);
			//������������ò���ѡ��Ԫ��˹��Ԫ�ⷽ����
		Hk_gradk = Hk.combine_columns(Hk, gradk);
		Gaussian_elimination_partial_pivoting(Hk_gradk, sk);
		//��ֽӽ����ݶ�Ϊ��ĵ�ǰ����������������ֽӽ��󲽳�Ϊ1					//���ǵ�һ��ֱ����sk��֮��ÿ��/2����������
		if (gradk.vector_2_norm() < 1e-3)	//���ݶȺ�С����ֽӽ�
			alpha0 = 1;
		else								//���������󲽳���alphaÿ������0.001��
		{
			ComplexMatrix sk_temp(sk.lr, sk.lc, sk.is_real, sk.c);	//����������������ʱ��ʱs
			ComplexMatrix xk_temp(xk.lr, xk.lc, xk.is_real, xk.c);	//����������������ʱ��ʱx
			alpha = 0.0;	//��ʼ������
			do {
				alpha0 = alpha;
				fx0 = f(xk_temp);
				alpha += 0.001;
				sk_temp = alpha * sk;
				xk_temp = xk_temp + sk_temp;
				fx = f(xk_temp);
			} while (fx < fx0);	//�ѳ�ʱfx >= fx0��fx0��Ӧ��alpha0Ϊ�����ҵ���Сֵ�Ĳ���
		}
		//������
		sk = alpha0 * sk;
		xk = xk + sk;					//cout << xk << endl;		//�������·��������ʱע�͵�
		//���¸��ݶ�
		for (int i = 0; i < xk.lr; i++)
			gradk.c[i][0].re = -((grad[i])(xk));
	//	dataOut << xk.c[0][0].re << " " << xk.c[1][0].re << endl;		//���x���ļ�������ʱע�͵�
	} while (gradk.vector_2_norm() >= 1e-15);	//���ݶ�Ϊ��ʱ�ѳ�								//��������try�Ƴ��Դ���
	//dataOut.close();													//������ļ�������ʱע�͵�
	cout << xk << endl;													//�������x������ʱע�͵�
	return f(xk);
}
//��Լ���Ż���BFGS������������ʵ����
double ComplexMatrix::BFGS_optimization(ComplexMatrix& x0, double(*f)(ComplexMatrix& x), double(*grad[])(ComplexMatrix& x))
{
	ComplexMatrix Bk = make_eyes(x0.lr);				//��ʼ��������Ϊ��λ��
	ComplexMatrix xk(x0);								//��ʼ��������
	ComplexMatrix yk(x0);								//�������������е���ʱ����
	ComplexMatrix sk(x0.lr, x0.lc, x0.is_real);				//���沽��
	ComplexMatrix gradk(x0.lr, x0.lc, x0.is_real);			//���渺�ݶ�
	ComplexMatrix gradk0(gradk);							//��ʱ������һ�εĸ��ݶ�
	ComplexMatrix Bk_gradk = combine_columns(Bk, gradk);	//�����������
	//��ʼ���ݶ�
	for (int i = 0; i < xk.lr; i++)
		gradk.c[i][0].re = -((grad[i])(xk));
	do {

		//������������ò���ѡ��Ԫ��˹��Ԫ�ⷽ����
		Bk_gradk = combine_columns(Bk, gradk);
		Gaussian_elimination_partial_pivoting(Bk_gradk, sk);
		//������
		xk = xk + sk;					cout << xk << endl;		//�������·��������ʱע�͵�
		//�����������ƾ���
		//���µĸ��ݶ�
		gradk0 = gradk;
		for (int i = 0; i < xk.lr; i++)
			gradk.c[i][0].re = -((grad[i])(xk));
		yk = gradk0 - gradk;

		ComplexMatrix ykT = (!yk);
		ComplexMatrix yk_ykT = yk * ykT;
		ComplexMatrix ykT_sk = ykT * sk;
		double ykT_sk_val = ykT_sk.c[0][0].re; ykT_sk_val = 1 / ykT_sk_val;
		yk_ykT = yk_ykT * ykT_sk_val;

		ComplexMatrix skT = (!sk);
		ComplexMatrix temp1 = Bk * sk;
		temp1 = temp1 * skT;
		temp1 = temp1 * Bk;
		ComplexMatrix temp2 = skT * Bk;
		temp2 = temp2 * sk;
		double temp2_val = temp2.c[0][0].re; temp2_val = 1 / temp2_val;
		temp1 = temp1 * temp2_val;

		Bk = Bk + yk_ykT;
		Bk = Bk - temp1;
	} while (gradk.vector_2_norm() >= 1e-15);			//���ݶ�Ϊ��ʱ�ѳ�								//��������try�Ƴ��Դ���
	cout << xk << endl;													//�������x������ʱע�͵�
	return f(xk);
}
//��Լ���Ż��Ĺ����ݶȷ���ʵ����beta��Fletcher-Reeves��ʽ����
double ComplexMatrix::Conjugate_Gradient_F_R(ComplexMatrix& x0, double(*f)(ComplexMatrix& x), double(*grad[])(ComplexMatrix& x))
{
	ComplexMatrix temp_0(x0.lr, x0.lc, x0.is_real);	//������
	ComplexMatrix temp1, temp2;						//�����м����
	double beta;									//������������ݶ���������
	double alpha0;										//������������һ�β���
	double alpha;										//������������
	double fx0;											//����������������һ�εĺ���ֵ
	double fx;											//�������������б��εĺ���ֵ
	ComplexMatrix xk(x0);							//��ʼ������
	ComplexMatrix gk(x0.lr, x0.lc, x0.is_real);		//��ʼ�ݶ�
	for (int i = 0; i < xk.lr; i++) gk.c[i][0].re = ((grad[i])(xk));
	ComplexMatrix gk0(gk);							//�ϴε����е��ݶ�
	ComplexMatrix gkT;								//�ݶ�ת��
	ComplexMatrix gk0T;								//�ϴε����е��ݶ�ת��
	ComplexMatrix sk(x0.lr, x0.lc, x0.is_real);		//��ʼ������
	sk = temp_0 - gk;
	ComplexMatrix sk0(sk);							//��һ�ε����еĲ���
	do {
		//���������󲽳�
		ComplexMatrix sk_temp(sk);	//����������������ʱ��ʱs
		ComplexMatrix xk_temp(xk);	//����������������ʱ��ʱx
		alpha = 0.0;	//��ʼ������
		do {
			alpha0 = alpha;
			fx0 = f(xk_temp);
			alpha += 0.001;
			sk_temp = alpha * sk;
			xk_temp = xk_temp + sk_temp;
			fx = f(xk_temp);
		} while (fx < fx0);	//�ѳ�ʱfx >= fx0��fx0��Ӧ��alpha0Ϊ�����ҵ���Сֵ�Ĳ���
		//������
		sk0 = sk;
		sk = alpha0 * sk;
		xk = xk + sk;		//cout << xk << endl;		//�������·��������ʱע�͵�
		//�������ݶ�
		gk0 = gk; gk0T = (!gk0);
		for (int i = 0; i < xk.lr; i++) gk.c[i][0].re = (grad[i])(xk);
		gkT = (!gk);
		//beta��Fletcher-Reeves��ʽ����
		temp1 = gkT * gk;
		temp2 = gk0T * gk0;
		beta = temp1.c[0][0].re / temp2.c[0][0].re;
		//�����ݶ�
		sk = beta * sk;
		sk = sk - gk;							//static int i = 0; i++; cout << i << " ";//�����������������ʱע�͵�
	}while (gk.vector_2_norm() >= 1e-15);				//�ݶ�Ϊ��ʱ�ѳ�								//��������try�Ƴ��Դ���
	cout << xk << endl;													//�������x������ʱע�͵�
	return f(xk);
}
//��Լ���Ż��Ĺ����ݶȷ���ʵ����beta��Polak-Ribiere��ʽ����
double ComplexMatrix::Conjugate_Gradient_P_R(ComplexMatrix& x0, double(*f)(ComplexMatrix& x), double(*grad[])(ComplexMatrix& x))
{
	ComplexMatrix temp_0(x0.lr, x0.lc, x0.is_real);	//������
	ComplexMatrix temp1, temp2;						//�����м����
	double beta;									//������������ݶ���������
	double alpha0;										//������������һ�β���
	double alpha;										//������������
	double fx0;											//����������������һ�εĺ���ֵ
	double fx;											//�������������б��εĺ���ֵ
	ComplexMatrix xk(x0);							//��ʼ������
	ComplexMatrix gk(x0.lr, x0.lc, x0.is_real);		//��ʼ�ݶ�
	for (int i = 0; i < xk.lr; i++) gk.c[i][0].re = ((grad[i])(xk));
	ComplexMatrix gk0(gk);							//�ϴε����е��ݶ�
	ComplexMatrix gk0T;								//�ϴε����е��ݶ�ת��
	ComplexMatrix sk(x0.lr, x0.lc, x0.is_real);		//��ʼ������
	sk = temp_0 - gk;
	ComplexMatrix sk0(sk);							//��һ�ε����еĲ���
	do {
		//���������󲽳�
		ComplexMatrix sk_temp(sk.lr, sk.lc, sk.is_real, sk.c);	//����������������ʱ��ʱs
		ComplexMatrix xk_temp(xk.lr, xk.lc, xk.is_real, xk.c);	//����������������ʱ��ʱx
		alpha = 0.0;	//��ʼ������
		do {
			alpha0 = alpha;
			fx0 = f(xk_temp);
			alpha += 0.001;
			sk_temp = alpha * sk;
			xk_temp = xk_temp + sk_temp;
			fx = f(xk_temp);
		} while (fx < fx0);	//�ѳ�ʱfx >= fx0��fx0��Ӧ��alpha0Ϊ�����ҵ���Сֵ�Ĳ���
		//������
		sk0 = sk;
		sk = alpha0 * sk;
		xk = xk + sk;		//cout << xk << endl;		//�������·��������ʱע�͵�
		//�������ݶ�
		gk0 = gk; gk0T = (!gk0);
		for (int i = 0; i < xk.lr; i++) gk.c[i][0].re = (grad[i])(xk);
		//beta��Polak-Ribiere��ʽ����
		temp1 = gk - gk0;
		temp1 = (!temp1);
		temp1 = temp1 * gk;
		temp2 = gk0T * gk0;
		beta = temp1.c[0][0].re / temp2.c[0][0].re;
		//�����ݶ�
		sk = beta * sk0;
		sk = sk - gk;			//static int i = 0; i++; cout << i << " ";//�����������������ʱע�͵�
	} while (sk.vector_2_norm() >= 1e-15);				//�ݶ�Ϊ��ʱ�ѳ�								//��������try�Ƴ��Դ���
	cout << xk << endl;													//�������x������ʱע�͵�
	return f(xk);
}
//��������С���ˣ���˹-ţ�ٷ���ʵ����
ComplexMatrix ComplexMatrix::Gauss_Newton_nonlinear_least_squares
(ComplexMatrix& x0, const ComplexMatrix& t, const ComplexMatrix& y,
	double(*f)(double, ComplexMatrix&), double(*Jacobian[])(double, ComplexMatrix&))
{
	ComplexMatrix xk(x0);						//��ʼ��������С���˽�
	ComplexMatrix sk(x0.lr, 1, x0.is_real);		//��������
	ComplexMatrix r(t.lr, 1, t.is_real);		//���в�����
	ComplexMatrix J(t.lr, x0.lr, t.is_real);	//�ſɱȾ���
	ComplexMatrix Jr;							//�����������
	for (int i = 0; i < t.lr; i++)				//�����ʼ���в�
		r.c[i][0].re = -y.c[i][0].re + f(t.c[i][0].re, xk);
	ComplexMatrix r0(r);						//ǰһ�����в�����
	ComplexMatrix r_r0 = r - r0;				//���в������ı仯��
	do {
		for (int i = 0; i < t.lr; i++)		//�����ſɱȾ���
			for (int j = 0; j < xk.lr; j++)
				J.c[i][j].re = Jacobian[j](t.c[i][0].re, xk);
		Jr = combine_columns(J, r);			//�����²�������С���˽�
		Householder_QR_augmented(Jr);
		solution_of_augmentedMatrix_after_QR(Jr, sk);
		xk = xk + sk;			cout << xk << endl;//��ʾ����·��������ʱע�͵�
		r0 = r;
		for (int i = 0; i < t.lr; i++)		//�����µĸ��в�
			r.c[i][0].re = -y.c[i][0].re + f(t.c[i][0].re, xk);
		r_r0 = r - r0;
	} while (r_r0.vector_2_norm() >= 1e-4);	//�в���������ڲ��ٱ仯ʱ�ѳ�
	return xk;
}
//��ʽԼ���Ż����������ճ��ӷ�����ζ��ι滮��ʵ����
double ComplexMatrix::Lagrange_multipliers_sequential_quadratic_programming(ComplexMatrix& x0, ComplexMatrix& lambda0,
	double(*f)(ComplexMatrix& x), double(*gradf[])(ComplexMatrix& x), double(*Hf[])(ComplexMatrix& x),
	double(*g[])(ComplexMatrix& x), double(*Jg[])(ComplexMatrix& x), double(*Hg[])(ComplexMatrix& x))
{
	ComplexMatrix xk(x0);							//��ʼ��������x
	ComplexMatrix lambdak(lambda0);					//��ʼ��������lambda
	ComplexMatrix xlk = combine_rows(xk, lambdak);	//��ʼ�������Է�����ĵ�����
	ComplexMatrix sk(xlk.lr, 1, x0.is_real);		//��ʼ����������Ϊ��
	//����ţ�ٲ��������Է�����
	ComplexMatrix B(x0.lr, x0.lr, x0.is_real);					//�����ʽ��ߵ�B
	ComplexMatrix J(lambda0.lr, x0.lr, x0.is_real);				//����g���ſɱȾ���
	ComplexMatrix JT = !J;										//����g�ſɱȾ����ת��
	ComplexMatrix A0(lambda0.lr, lambda0.lr, x0.is_real);		//��ʽ��߾��������㲿��
	ComplexMatrix A;											//�����ʽ��ߵ�ϵ������
	ComplexMatrix b1(x0.lr, 1, x0.is_real);			//�����ʽ�ұߵ������ϰ벿��
	ComplexMatrix b2(lambda0.lr, 1, x0.is_real);	//�����ʽ�ұߵ������°벿��
	ComplexMatrix b;								//�����ʽ�ұߵ�����
	ComplexMatrix Ab;						//�����������
	ComplexMatrix temp0(x0.lr, 1);			//��ʱ������
	ComplexMatrix temp1;					//�����м����
	do {
		//������
		xlk = xlk + sk;
		xk = xlk.get_rows(0, x0.lr - 1);
		lambdak = xlk.get_rows(x0.lr, xlk.lr - 1);		cout << xlk << endl;
		//���ʽ��ߵ�ϵ������
			//���ʽ������ϵ�B
		for (int i = 0; i < B.lr; i++)
			for (int j = 0; j < B.lc; j++)
			{
				B.c[i][j].re = (Hf[i * x0.lr + j])(xk);
				for (int m = 0; m < lambdak.lr; m++)
					B.c[i][j].re += (lambdak.c[m][0].re * (Hg[m * x0.lr * x0.lr + i * x0.lr + j])(xk));
			}
			//��g���ſɱȾ�����ת��
		for (int i = 0; i < J.lr; i++)
			for (int j = 0; j < J.lc; j++)
				J.c[i][j].re = (Jg[i * x0.lr + j])(xk);
		JT = !J;
			//ƴ�ɵ�ʽ��ߵ�ϵ������
		A = combine_columns(B, JT);
		temp1 = combine_columns(J, A0);
		A = combine_rows(A, temp1);
		//���ʽ�ұߵ�����
			//���ϰ벿��
		temp1 = JT * lambdak;
		for (int i = 0; i < b1.lr; i++)
			b1.c[i][0].re = (gradf[i])(xk);
		b1 = b1 + temp1;
		b1 = temp0 - b1;
			//���°벿��
		for (int i = 0; i < b2.lr; i++)
			b2.c[i][0].re = -((g[i])(xk));
			//ƴ���ұߵ�����
		b = combine_rows(b1, b2);
		//ƴ��������󲢽ⲽ��
		Ab = combine_columns(A, b);
		Gaussian_elimination_partial_pivoting(Ab, sk);
	} while (sk.vector_2_norm() >= 1e-15);		//�������������Ϊ��ʱ�ѳ�
	xk = xlk.get_rows(0, x0.lr - 1);
	return f(xk);
}