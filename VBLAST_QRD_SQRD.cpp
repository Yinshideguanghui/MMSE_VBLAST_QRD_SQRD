#define _CRT_SECURE_NO_WARNINGS
#include"complex_matrix_operators.h"

//************************************��������**********************************************

//���һ����ֵΪ�㣬����Ϊ1�ĸ�˹����ֵ���˦ң��Ӧ̣��ɱ�Ϊ��׼��ң���ֵ�̵ĸ�˹����
//ʹ�õ�Box-Muller�㷨������ѧ��MATLAB���漰
//����ο�:https://blog.csdn.net/a616905919/article/details/45080539
double gaussianrand()
{
	static double V1, V2, S;
	static int phase = 0;
	double X;
	if (phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;
			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while (S >= 1 || S == 0);	//�����漰���������ȥS=0������������Ӱ��������������ķֲ�
		X = V1 * sqrt(-2 * log(S) / S);
	}
	else
		X = V2 * sqrt(-2 * log(S) / S);
	phase = 1 - phase;	//����X�����Ӿ�ֵΪ0������Ϊ1�ĸ�˹�ֲ�����ÿ��ѡ����һ�����´�ѡ��һ��
	return X;
}


//************************************�����źš��ŵ�������************************************

//��ʵ���еĵ����źŹ���
Complex estimating_quantization_operation(Complex y)
{
	Complex result;
	//��һ���޺�����ʵ���ϵĹ���Ϊ(1+j)/sqrt(2)
	if ((y.re > 0) && (y.im >= 0)) { result.re = 1.0 / sqrt(2.0); result.im = 1.0 / sqrt(2.0); }
	//�ڶ����޺����������ϵĹ���Ϊ(-1+j)/sqrt(2)
	else if ((y.re <= 0) && (y.im > 0)) { result.re = -1.0 / sqrt(2.0); result.im = 1.0 / sqrt(2.0); }
	//�������޺͸���ʵ���ϵĹ���Ϊ(-1-j)/sqrt(2)
	else if ((y.re < 0) && (y.im <= 0)) { result.re = -1.0 / sqrt(2.0); result.im = -1.0 / sqrt(2.0); }
	//�������޺͸��������ϵĹ���Ϊ(1-j)/sqrt(2)
	else if ((y.re >= 0) && (y.im < 0)) { result.re = 1.0 / sqrt(2.0); result.im = -1.0 / sqrt(2.0); }
	//ԭ���ϵĹ���Ϊ1+j
	else { result.re = 1.0 / sqrt(2.0); result.im = 1.0 / sqrt(2.0); }
	return result;
}

//�������ȷֲ������01���У�ע���������nT���Ժ���ƺ�ķ��������������ܱ�����ΪnT������
int* generate_bits(int nT)
{
	double* random_nums = new double[nT * 2];
	int* result = new int[nT * 2];
	for (int i = 0; i < nT * 2; i++)
	{
		random_nums[i] = ((double)rand() / RAND_MAX) * 2 - 1;
		if (random_nums[i] >= 0)result[i] = 1;
		else result[i] = 0;
	}
	delete[]random_nums;
	return result;
}

//��������ɵ�bits���ƣ�00��1+j��01��1-j��10��-1+j��11��-1-j�����Ҷ�/sqrt(2)
ComplexMatrix generate_signal(int* bits, int nT)
{
	ComplexMatrix signal(nT, 1, false);
	for (int i = 0; i < nT * 2; i++)
	{
		signal.c[i/2][0].re = bits[i] ? (-1 / sqrt(2.0)) : 1 / sqrt(2.0);
		i++;
		signal.c[i/2][0].im = bits[i] ? (-1 / sqrt(2.0)) : 1 / sqrt(2.0);
	}
	return signal;
}

//�ѻ�ԭ�����źŷ����bits
int* signal_to_bits(ComplexMatrix& c_hat)
{
	int* bits = new int[c_hat.lr * 2];
	for (int i = 0; i < c_hat.lr * 2; i++)
	{
		bits[i] = (abs(c_hat.c[i / 2][0].re + 1 / sqrt(2.0)) < 1e-16) ? 1 : 0;
		i++;
		bits[i] = (abs(c_hat.c[i / 2][0].im + 1 / sqrt(2.0)) < 1e-16) ? 1 : 0;
	}
	return bits;
}

//����һ����˹��������ŵ����󣨾�ֵ0������1�����������
//���ݱ���ʹ�õĶ��壬ʵ���鲿�ֱ�Ϊ����ֵ0������1���Ķ�����˹�����ĸ���Ϊ����ֵ0������1���ĸ���˹����
ComplexMatrix generate_H(int nR, int nT)
{
	ComplexMatrix H(nR, nT, false);
	for(int i = 0; i < H.lr; i++)
		for (int j = 0; j < H.lc; j++)
		{
			H.c[i][j].re = gaussianrand();
			H.c[i][j].im = gaussianrand();
		}
	return H;
}

//����һ����˹�������������������ֵ0������1�����������
//���ݱ���ʹ�õĶ��壬ʵ���鲿�ֱ�Ϊ����ֵ0������1���Ķ�����˹�����ĸ���Ϊ����ֵ0������1���ĸ���˹����
ComplexMatrix generate_noise(int nR)
{
	ComplexMatrix niu(nR, 1, false);
	for (int i = 0; i < niu.lr; i++)
	{
		niu.c[i][0].re = gaussianrand();
		niu.c[i][0].im = gaussianrand();
	}
	return niu;
}

//************************************��ԭ�㷨**********************************************

//ֱ�ӳ�α���㷨
ComplexMatrix Pseudo_inverse(ComplexMatrix& x, ComplexMatrix& H)
{
	//����x:nR * 1		//�ŵ�H:nR * nT		//��ԭ��c_hat:nT * 1
	ComplexMatrix x_temp(x.lr, x.lc, x.is_real, x.c);	//�����źŴ�����x����ʱ��������ֹԭx����
	ComplexMatrix c_hat(H.lc, 1, x.is_real);			//����Ļ�ԭ���ķ����ź�
	ComplexMatrix G = H.Moore_Penrose_pseudo_inverse();	//H��α��
	x_temp = G * x_temp;
	for (int i = 0; i < H.lc; i++)c_hat.c[i][0] = estimating_quantization_operation(x_temp.c[i][0]);
	//�ͷ��ڴ�
	//x_temp.clear(); G.clear();;
	return c_hat;
}

//V-BLAST�㷨
ComplexMatrix V_BLAST(ComplexMatrix& x, ComplexMatrix& H)
{
	//����x:nR * 1		//�ŵ�H:nR * nT		//��ԭ��c_hat:nT * 1
	ComplexMatrix x_temp(x.lr, x.lc, x.is_real, x.c);	//�����źŴ�����x����ʱ��������ֹԭx����
	ComplexMatrix c_hat(H.lc, 1, x.is_real);			//����Ļ�ԭ���ķ����ź�
	ComplexMatrix y(1, 1);								//��ԭ���ķ����źź�����֮��
	Complex y_ = 0.0;									//��ԭ���ķ����źź�����֮��
	ComplexMatrix G;									//H��α��
	ComplexMatrix g;									//�洢g����
	double g_2_norm;									//�洢g��2-����
	double g_2_norm_min = 0.0;							//�洢G��2-������С��һ�е�2-����
	int g_2_norm_min_row = 0;							//�洢G��2-������С��һ�еĴ洢�±�
	ComplexMatrix H_temp;								//�洢H�ڵ���������ÿ�μ�һ�к�Ľ��
	ComplexMatrix h;									//�洢H_temp����
	ComplexMatrix temp;									//�����н�
	//��¼H�����Ƿ��ѱ������Ա�����һ����Hȥ����Ӧ��
	int** flag1 = new int* [2];
	flag1[0] = new int[H.lc];
	flag1[1] = new int[H.lc];
	for (int i = 0; i < H.lc; i++)
	{
		flag1[0][i] = i;	//flag1��һ�м�¼���±�
		flag1[1][i] = 0;	//flag1�ڶ��м�¼��ǰ���Ƿ��ѱ�������ʼΪ0(δ����)
	}
	int k = 0;	//��¼flag2��ֵ����һ����

	//��ѭ��nT��
	for (int i = 0; i < H.lc; i++)
	{
		//����flag1ƴ��ȥ���Ѵ����е���H���ͼ�¼��ǰʣ�µ��ж�Ӧ��ԭ�±��flag2
		int* flag2 = new int[H.lc - i];
		k = 0;
		for (int j = 0; j < H.lc; j++)
			if (!flag1[1][j])
			{
				flag2[k] = flag1[0][j];
				k++;
			}
		H_temp = H.get_column(flag2[0]);
		for (int j = 1; j < H.lc - i; j++)
		{
			h = H.get_column(flag2[j]);
			H_temp = H_temp.combine_columns(H_temp, h);
		}
		//��H_temp��α��G
		G = H_temp.Moore_Penrose_pseudo_inverse();
		//�ҳ�G��2-������С��һ�У���ʼΪ��һ�У�
		g_2_norm_min_row = 0;
		g = G.get_row(0);
		g_2_norm_min = g.vector_2_norm();
		for (int j = 1; j < G.lr; j++)
		{
			g = G.get_row(j);
			g_2_norm = g.vector_2_norm();
			if (g_2_norm < g_2_norm_min)
			{
				g_2_norm_min_row = j;
				g_2_norm_min = g_2_norm;
			}
		}
		//ȡ��G��2-������С��һ��
		g = G.get_row(g_2_norm_min_row);
		//�����źŻ�ԭ
		y = g * x_temp;
		//��ԭ���ź����������ӹ��㣨������ƣ���ʵ��������з�Χ�ڲ���Խ�磩
		c_hat.c[flag2[g_2_norm_min_row]][0] = estimating_quantization_operation(y.c[0][0]);
		//�ù���Ļ�ԭ�ź����������ź�x_temp
		h = H_temp.get_column(g_2_norm_min_row);
		temp = h * c_hat.c[flag2[g_2_norm_min_row]][0];
		x_temp = x_temp - temp;
		//flag1�е�ǰ�����һ�б��Ϊ�Ѵ���
		flag1[1][flag2[g_2_norm_min_row]] = 1;
		//�ͷ��ڴ�
		delete[]flag2;
	}
	//�ͷ��ڴ�
	delete[]flag1[1];	delete[]flag1[0];	delete[]flag1;
	//x_temp.clear();	y.clear(); G.clear(); g.clear(); H_temp.clear(); h.clear(); temp.clear();
	return c_hat;
}

//V-BLAST�ֲ������ʼ��
ComplexMatrix V_BLAST_layer(ComplexMatrix& x, ComplexMatrix& H, int* bits)
{
	//����x:nR * 1		//�ŵ�H:nR * nT		//��ԭ��c_hat:nT * 1
	ComplexMatrix x_temp(x.lr, x.lc, x.is_real, x.c);	//�����źŴ�����x����ʱ��������ֹԭx����
	ComplexMatrix c_hat(H.lc, 1, x.is_real);			//����Ļ�ԭ���ķ����ź�
	ComplexMatrix y(1, 1);								//��ԭ���ķ����źź�����֮��
	Complex y_ = 0.0;									//��ԭ���ķ����źź�����֮��
	ComplexMatrix G;									//H��α��
	ComplexMatrix g;									//�洢g����
	double g_2_norm;									//�洢g��2-����
	double g_2_norm_min = 0.0;							//�洢G��2-������С��һ�е�2-����
	int g_2_norm_min_row = 0;							//�洢G��2-������С��һ�еĴ洢�±�
	ComplexMatrix H_temp;								//�洢H�ڵ���������ÿ�μ�һ�к�Ľ��
	ComplexMatrix h;									//�洢H_temp����
	ComplexMatrix temp;									//�����н�

	//��¼H�����Ƿ��ѱ������Ա�����һ����Hȥ����Ӧ��
	int** flag1 = new int* [3];
	flag1[0] = new int[H.lc];
	flag1[1] = new int[H.lc];
	flag1[2] = new int[H.lc];
	for (int i = 0; i < H.lc; i++)
	{
		flag1[0][i] = i;	//flag1��һ�м�¼���±�
		flag1[1][i] = 0;	//flag1�ڶ��м�¼��ǰ���Ƿ��ѱ�������ʼΪ0(δ����)
		flag1[2][i] = 0;	//flag1�����м�¼��ǰ���ǵڼ����������
	}
	int k = 0;	//��¼flag2��ֵ����һ����

	//��ѭ��nT��
	for (int i = 0; i < H.lc; i++)
	{
		//����flag1ƴ��ȥ���Ѵ����е���H���ͼ�¼��ǰʣ�µ��ж�Ӧ��ԭ�±��flag2
		int* flag2 = new int[H.lc - i];
		k = 0;
		for (int j = 0; j < H.lc; j++)
			if (!flag1[1][j])
			{
				flag2[k] = flag1[0][j];
				k++;
			}
		H_temp = H.get_column(flag2[0]);
		for (int j = 1; j < H.lc - i; j++)
		{
			h = H.get_column(flag2[j]);
			H_temp = H_temp.combine_columns(H_temp, h);
		}
		//��H_temp��α��G
		G = H_temp.Moore_Penrose_pseudo_inverse();
		//�ҳ�G��2-������С��һ�У���ʼΪ��һ�У�
		g_2_norm_min_row = 0;
		g = G.get_row(0);
		g_2_norm_min = g.vector_2_norm();
		for (int j = 1; j < G.lr; j++)
		{
			g = G.get_row(j);
			g_2_norm = g.vector_2_norm();
			if (g_2_norm < g_2_norm_min)
			{
				g_2_norm_min_row = j;
				g_2_norm_min = g_2_norm;
			}
		}
		//ȡ��G��2-������С��һ��
		g = G.get_row(g_2_norm_min_row);
		//�����źŻ�ԭ
		y = g * x_temp;
		//��ԭ���ź����������ӹ��㣨������ƣ���ʵ��������з�Χ�ڲ���Խ�磩
		c_hat.c[flag2[g_2_norm_min_row]][0] = estimating_quantization_operation(y.c[0][0]);
		//�ù���Ļ�ԭ�ź����������ź�x_temp
		h = H_temp.get_column(g_2_norm_min_row);
		temp = h * c_hat.c[flag2[g_2_norm_min_row]][0];
		x_temp = x_temp - temp;
		//flag1�е�ǰ�����һ�б��Ϊ�Ѵ�������¼������˳��
		flag1[1][flag2[g_2_norm_min_row]] = 1;
		flag1[2][flag2[g_2_norm_min_row]] = i;
		//�ͷ��ڴ�
		delete[]flag2;
	}
	//��ԭ�źŷ���ر�����
	int* bits_hat = signal_to_bits(c_hat);
	//���ռ�¼�Ĵ���˳��ִ�зֲ������ʼ�⣬����Ϊ��߲�
	ComplexMatrix BER_per_layer(1, H.lc, true);			//��ʼ���ֲ������ʼ�¼����
	for (int i = 0; i < H.lc; i++)						//��nT��
	{
		int order = flag1[2][i];		//��i�еĴ���˳λ(0��ʼ)
		int layer = H.lc - order;		//��i�еĲ���(��0�������Ӧ��nT�㣬֮����˳λÿ+1������-1)
		//��ȡ��i��ԭʼ��2λ����
		int bit0 = bits[2 * i];
		int bit1 = bits[2 * i + 1];
		//��ȡ��i�з���ص�2λ����
		int bit0_hat = bits_hat[2 * i];
		int bit1_hat = bits_hat[2 * i + 1];
		//��i���ǵ�layer�㣬�����������ʼ��ĵ�(layer-1)��
		//�����ʣ�����һ�����1��������2
		BER_per_layer.c[0][layer - 1].re += (bit0 != bit0_hat);
		BER_per_layer.c[0][layer - 1].re += (bit1 != bit1_hat);
		BER_per_layer.c[0][layer - 1].re /= 2.0;
	}
	//�ͷ��ڴ�
	delete[] flag1[2]; delete[]flag1[1]; delete[]flag1[0];	delete[]flag1;	delete[] bits_hat;
	//x_temp.clear();	y.clear(); G.clear(); g.clear(); H_temp.clear(); h.clear(); temp.clear();
	return BER_per_layer;
}

//������QRD�㷨
ComplexMatrix QRD(ComplexMatrix& x, ComplexMatrix& H)
{
	//����x:nR * 1		//�ŵ�H:nR * nT		//��ԭ��c_hat:nT * 1
	ComplexMatrix c_hat(H.lc, 1, x.is_real);		//����Ļ�ԭ���ķ����ź�
	ComplexMatrix H_temp(H.lr, H.lc, H.is_real, H.c);	//�洢H�ڱ���V-BLAST�����е���ʱ����
	ComplexMatrix y;		//�洢�����ź�x��Q��������ź�
	ComplexMatrix Q_H;		//�洢Q�Ĺ���ת��
	Complex temp;		//�����н�
	//��H_temp����QR�ֽ⣬֮��H_temp�д洢��Q
	ComplexMatrix R = H_temp.Gram_Schmidt_QR_modified__Complex(H_temp);
	//x����Q�任Ϊy
	Q_H = !H_temp;
	y = Q_H * x;
	//����R��nT�к�y��ԭ�������ź�
	for (int i = c_hat.lr - 1; i >= 0; i--)
	{
		c_hat.c[i][0] = y.c[i][0];
		for (int j = c_hat.lr - 1; j > i; j--)
		{
			temp = R.c[i][j] * c_hat.c[j][0];
			c_hat.c[i][0] = c_hat.c[i][0] - temp;
		}
		c_hat.c[i][0] = c_hat.c[i][0] / R.c[i][i];
		c_hat.c[i][0] = estimating_quantization_operation(c_hat.c[i][0]);
	}
	//�ͷ��ڴ�
	//H_temp.clear();	y.clear(); Q_H.clear(); R.clear();
	return c_hat;
}

//SQRD�㷨
ComplexMatrix SQRD(ComplexMatrix& x, ComplexMatrix& H)
{
	//����x:nR * 1		//�ŵ�H:nR * nT		//��ԭ��c_hat:nT * 1
	ComplexMatrix c_hat(H.lc, 1, x.is_real);		//����Ļ�ԭ���ķ����ź�
	ComplexMatrix c_hat_temp(H.lc, 1, x.is_real);	//����Ļ�ԭ���ķ����źŵ���ʱ����
	ComplexMatrix H_temp(H.lr, H.lc, H.is_real, H.c);	//�洢H�ڱ���V-BLAST�����е���ʱ����
	ComplexMatrix y;		//�洢�����ź�x��Q��������ź�
	ComplexMatrix Q_H;		//�洢Q�Ĺ���ת��
	Complex temp;		//�����н�
	//��ʼ���洢������¼������
	int* S = new int[H.lc];
	for (int j = 0; j < H.lc; j++) S[j] = j;
	//��H_temp�����������QR�ֽ⣬֮��H_temp�д洢��Q
	ComplexMatrix R = H_temp.sorted_Gram_Schmidt_QR_modified__Complex(H_temp, S);
	//x����Q�任Ϊy
	Q_H = !H_temp;
	y = Q_H * x;
	//����R��nT�к�y��ԭ�������ź�
	for (int i = H.lc - 1; i >= 0; i--)
	{
		c_hat.c[i][0] = y.c[i][0];
		for (int j = H.lc - 1; j > i; j--)
		{
			temp = R.c[i][j] * c_hat.c[j][0];
			c_hat.c[i][0] = c_hat.c[i][0] - temp;
		}
		c_hat.c[i][0] = c_hat.c[i][0] / R.c[i][i];
		c_hat.c[i][0] = estimating_quantization_operation(c_hat.c[i][0]);
	}
	//��S�л�ԭ�����ź�c��ԭ˳���ȸ���һ�ݣ��ٰ�S˳�򽫸���Ʒ�е�Ԫ����������������
	for (int i = 0; i < H.lc; i++) c_hat_temp.c[i][0] = c_hat.c[i][0];
	for (int i = 0; i < H.lc; i++) c_hat.c[S[i]][0] = c_hat_temp.c[i][0];
	//�ͷ��ڴ�
	delete[]S;
	//c_hat_temp.clear(); H_temp.clear();	y.clear(); Q_H.clear(); R.clear();
	return c_hat;
}

//************************************MMSE��ԭ�㷨******************************************

//MMSE-����(ֱ�ӳ�α��)�㷨
ComplexMatrix MMSE_Pseudo_inverse(ComplexMatrix& x, ComplexMatrix& H)
{
	//����x:nR * 1		//�ŵ�H:nR * nT		//��ԭ��c_hat:nT * 1
	ComplexMatrix x_temp(x.lr, x.lc, x.is_real, x.c);	//�����źŴ�����x����ʱ��������ֹԭx����
	ComplexMatrix x0(H.lc, 1, x.is_real);				//����ƴ�ӵ���ʱ������
	x_temp = x_temp.combine_rows(x_temp, x0);			//MMSE��������ý�������
	ComplexMatrix c_hat(H.lc, 1, x.is_real);			//�������Ļ�ԭ���ķ����ź�

	ComplexMatrix H_temp(H);							//H����ʱ����
	ComplexMatrix I0 = H.make_eyes(H.lc);				//��λ��
	H_temp = H_temp.combine_rows(H_temp, I0);			//MMSE���������H����
	ComplexMatrix G = H_temp.Moore_Penrose_pseudo_inverse();	//MMSE-H��α��

	x_temp = G * x_temp;
	for (int i = 0; i < H.lc; i++)c_hat.c[i][0] = estimating_quantization_operation(x_temp.c[i][0]);
	//�ͷ��ڴ�
	//x_temp.clear(); G.clear();;
	return c_hat;
}

//MMSE-V-BLAST�㷨
ComplexMatrix MMSE_V_BLAST(ComplexMatrix& x, ComplexMatrix& H)
{
	//����x:nR * 1		//�ŵ�H:nR * nT		//��ԭ��c_hat:nT * 1
	ComplexMatrix x_temp(x.lr, x.lc, x.is_real, x.c);	//�����źŴ�����x����ʱ��������ֹԭx����
	ComplexMatrix x0(H.lc, 1, x.is_real);				//����ƴ�ӵ���ʱ������
	x_temp = x_temp.combine_rows(x_temp, x0);			//MMSE��������ý�������
	ComplexMatrix c_hat(H.lc, 1, x.is_real);			//����Ļ�ԭ���ķ����ź�

	ComplexMatrix I0 = H.make_eyes(H.lc);				//��λ��
	ComplexMatrix H_temp0 = H.combine_rows(H, I0);		//MMSE����������ŵ�����

	ComplexMatrix y(1, 1);								//��ԭ���ķ����źź�����֮��
	Complex y_ = 0.0;									//��ԭ���ķ����źź�����֮��
	ComplexMatrix G;									//MMSE-H��α��
	ComplexMatrix g;									//�洢g����
	double g_2_norm;									//�洢g��2-����
	double g_2_norm_min = 0.0;							//�洢G��2-������С��һ�е�2-����
	int g_2_norm_min_row = 0;							//�洢G��2-������С��һ�еĴ洢�±�
	ComplexMatrix H_temp;								//�洢H�ڵ���������ÿ�μ�һ�к�Ľ��
	ComplexMatrix h;									//�洢H_temp����
	ComplexMatrix temp;									//�����н�
	//��¼H�����Ƿ��ѱ������Ա�����һ����Hȥ����Ӧ��
	int** flag1 = new int* [2];
	flag1[0] = new int[H_temp0.lc];
	flag1[1] = new int[H_temp0.lc];
	for (int i = 0; i < H_temp0.lc; i++)
	{
		flag1[0][i] = i;	//flag1��һ�м�¼���±�
		flag1[1][i] = 0;	//flag1�ڶ��м�¼��ǰ���Ƿ��ѱ�������ʼΪ0(δ����)
	}
	int k = 0;	//��¼flag2��ֵ����һ����

	//��ѭ��nT��
	for (int i = 0; i < H_temp0.lc; i++)
	{
		//����flag1ƴ��ȥ���Ѵ����е���H��ȥ��Ӧ�е�x���ͼ�¼��ǰʣ�µ��ж�Ӧ��ԭ�±��flag2
		int* flag2 = new int[H_temp0.lc - i];
		k = 0;
		for (int j = 0; j < H_temp0.lc; j++)
			if (!flag1[1][j])
			{
				flag2[k] = flag1[0][j];
				k++;
			}
		H_temp = H_temp0.get_column(flag2[0]);
		for (int j = 1; j < H_temp0.lc - i; j++)
		{
			h = H_temp0.get_column(flag2[j]);
			H_temp = H_temp.combine_columns(H_temp, h);
		}
		//��H_temp��α��G
		G = H_temp.Moore_Penrose_pseudo_inverse();
		//�ҳ�G��2-������С��һ�У���ʼΪ��һ�У�
		g_2_norm_min_row = 0;
		g = G.get_row(0);
		g_2_norm_min = g.vector_2_norm();
		for (int j = 1; j < G.lr; j++)
		{
			g = G.get_row(j);
			g_2_norm = g.vector_2_norm();
			if (g_2_norm < g_2_norm_min)
			{
				g_2_norm_min_row = j;
				g_2_norm_min = g_2_norm;
			}
		}
		//ȡ��G��2-������С��һ��
		g = G.get_row(g_2_norm_min_row);
		//�����źŻ�ԭ
		y = g * x_temp;
		//��ԭ���ź����������ӹ��㣨������ƣ���ʵ��������з�Χ�ڲ���Խ�磩
		c_hat.c[flag2[g_2_norm_min_row]][0] = estimating_quantization_operation(y.c[0][0]);
		//�ù���Ļ�ԭ�ź����������ź�x_temp
		h = H_temp.get_column(g_2_norm_min_row);
		temp = h * c_hat.c[flag2[g_2_norm_min_row]][0];
		x_temp = x_temp - temp;
		//flag1�е�ǰ�����һ�б��Ϊ�Ѵ���
		flag1[1][flag2[g_2_norm_min_row]] = 1;
		//�ͷ��ڴ�
		delete[]flag2;
	}
	//�ͷ��ڴ�
	delete[]flag1[1];	delete[]flag1[0];	delete[]flag1;
	//x_temp.clear();	y.clear(); G.clear(); g.clear(); H_temp.clear(); h.clear(); temp.clear();
	return c_hat;
}

//MMSE-V-BLAST�ֲ������ʼ��
ComplexMatrix MMSE_V_BLAST_layer(ComplexMatrix& x, ComplexMatrix& H, int* bits)
{
	//����x:nR * 1		//�ŵ�H:nR * nT		//��ԭ��c_hat:nT * 1
	ComplexMatrix x_temp(x.lr, x.lc, x.is_real, x.c);	//�����źŴ�����x����ʱ��������ֹԭx����
	ComplexMatrix x0(H.lc, 1, x.is_real);				//����ƴ�ӵ���ʱ������
	x_temp = x_temp.combine_rows(x_temp, x0);			//MMSE��������ý�������
	ComplexMatrix c_hat(H.lc, 1, x.is_real);			//����Ļ�ԭ���ķ����ź�

	ComplexMatrix I0 = H.make_eyes(H.lc);				//��λ��
	ComplexMatrix H_temp0 = H.combine_rows(H, I0);		//MMSE����������ŵ�����

	ComplexMatrix y(1, 1);								//��ԭ���ķ����źź�����֮��
	Complex y_ = 0.0;									//��ԭ���ķ����źź�����֮��
	ComplexMatrix G;									//MMSE-H��α��
	ComplexMatrix g;									//�洢g����
	double g_2_norm;									//�洢g��2-����
	double g_2_norm_min = 0.0;							//�洢G��2-������С��һ�е�2-����
	int g_2_norm_min_row = 0;							//�洢G��2-������С��һ�еĴ洢�±�
	ComplexMatrix H_temp;								//�洢H�ڵ���������ÿ�μ�һ�к�Ľ��
	ComplexMatrix h;									//�洢H_temp����
	ComplexMatrix temp;									//�����н�
	//��¼H�����Ƿ��ѱ������Ա�����һ����Hȥ����Ӧ��
	int** flag1 = new int* [3];
	flag1[0] = new int[H_temp0.lc];
	flag1[1] = new int[H_temp0.lc];
	flag1[2] = new int[H_temp0.lc];
	for (int i = 0; i < H_temp0.lc; i++)
	{
		flag1[0][i] = i;	//flag1��һ�м�¼���±�
		flag1[1][i] = 0;	//flag1�ڶ��м�¼��ǰ���Ƿ��ѱ�������ʼΪ0(δ����)
		flag1[2][i] = 0;	//flag1�����м�¼��ǰ���ǵڼ����������
	}
	int k = 0;	//��¼flag2��ֵ����һ����

	//��ѭ��nT��
	for (int i = 0; i < H_temp0.lc; i++)
	{
		//����flag1ƴ��ȥ���Ѵ����е���H��ȥ��Ӧ�е�x���ͼ�¼��ǰʣ�µ��ж�Ӧ��ԭ�±��flag2
		int* flag2 = new int[H_temp0.lc - i];
		k = 0;
		for (int j = 0; j < H_temp0.lc; j++)
			if (!flag1[1][j])
			{
				flag2[k] = flag1[0][j];
				k++;
			}
		H_temp = H_temp0.get_column(flag2[0]);
		for (int j = 1; j < H_temp0.lc - i; j++)
		{
			h = H_temp0.get_column(flag2[j]);
			H_temp = H_temp.combine_columns(H_temp, h);
		}
		//��H_temp��α��G
		G = H_temp.Moore_Penrose_pseudo_inverse();
		//�ҳ�G��2-������С��һ�У���ʼΪ��һ�У�
		g_2_norm_min_row = 0;
		g = G.get_row(0);
		g_2_norm_min = g.vector_2_norm();
		for (int j = 1; j < G.lr; j++)
		{
			g = G.get_row(j);
			g_2_norm = g.vector_2_norm();
			if (g_2_norm < g_2_norm_min)
			{
				g_2_norm_min_row = j;
				g_2_norm_min = g_2_norm;
			}
		}
		//ȡ��G��2-������С��һ��
		g = G.get_row(g_2_norm_min_row);
		//�����źŻ�ԭ
		y = g * x_temp;
		//��ԭ���ź����������ӹ��㣨������ƣ���ʵ��������з�Χ�ڲ���Խ�磩
		c_hat.c[flag2[g_2_norm_min_row]][0] = estimating_quantization_operation(y.c[0][0]);
		//�ù���Ļ�ԭ�ź����������ź�x_temp
		h = H_temp.get_column(g_2_norm_min_row);
		temp = h * c_hat.c[flag2[g_2_norm_min_row]][0];
		x_temp = x_temp - temp;
		//flag1�е�ǰ�����һ�б��Ϊ�Ѵ���
		flag1[1][flag2[g_2_norm_min_row]] = 1;
		flag1[2][flag2[g_2_norm_min_row]] = i;
		//�ͷ��ڴ�
		delete[]flag2;
	}
	//��ԭ�źŷ���ر�����
	int* bits_hat = signal_to_bits(c_hat);
	//���ռ�¼�Ĵ���˳��ִ�зֲ������ʼ�⣬����Ϊ��߲�
	ComplexMatrix BER_per_layer(1, H_temp0.lc, true);	//��ʼ���ֲ������ʼ�¼����
	for (int i = 0; i < H_temp0.lc; i++)				//��nT��
	{
		int order = flag1[2][i];		//��i�еĴ���˳λ(0��ʼ)
		int layer = H.lc - order;		//��i�еĲ���(��0�������Ӧ��nT�㣬֮����˳λÿ+1������-1)
		//��ȡ��i��ԭʼ��2λ����
		int bit0 = bits[2 * i];
		int bit1 = bits[2 * i + 1];
		//��ȡ��i�з���ص�2λ����
		int bit0_hat = bits_hat[2 * i];
		int bit1_hat = bits_hat[2 * i + 1];
		//��i���ǵ�layer�㣬�����������ʼ��ĵ�(layer-1)��
		//�����ʣ�����һ�����1��������2
		BER_per_layer.c[0][layer - 1].re += (bit0 != bit0_hat);
		BER_per_layer.c[0][layer - 1].re += (bit1 != bit1_hat);
		BER_per_layer.c[0][layer - 1].re /= 2.0;
	}
	//�ͷ��ڴ�
	delete[] flag1[2]; delete[]flag1[1]; delete[]flag1[0];	delete[]flag1;	delete[] bits_hat;
	return BER_per_layer;
}

//MMSE-������QRD�㷨
ComplexMatrix MMSE_QRD(ComplexMatrix& x, ComplexMatrix& H, double sigma_n)
{
	//����x:nR * 1		//�ŵ�H:nR * nT		//��ԭ��c_hat:nT * 1
	ComplexMatrix c_hat(H.lc, 1, x.is_real);		//����Ļ�ԭ���ķ����ź�
	ComplexMatrix H_temp(H.lr, H.lc, H.is_real, H.c);	//�洢H�ڱ���V-BLAST�����е���ʱ����
	ComplexMatrix I0 = H.make_eyes(H.lc);				//��λ��
	I0 = I0 * sigma_n;
	H_temp = H_temp.combine_rows(H_temp, I0);			//ƴ������H����
	ComplexMatrix y;		//�洢�����ź�x��Q��������ź�
	ComplexMatrix Q_H;		//�洢Q�Ĺ���ת��
	Complex temp;		//�����н�
	//��H_temp����QR�ֽ⣬֮��H_temp�д洢��Q
	ComplexMatrix R = H_temp.Gram_Schmidt_QR_modified__Complex(H_temp);
	//ȡ��Q1��x����Q�任Ϊy
	H_temp = H_temp.get_rows(0, H.lr - 1);
	Q_H = !H_temp;
	y = Q_H * x;
	//����R��nT�к�y��ԭ�������ź�
	for (int i = c_hat.lr - 1; i >= 0; i--)
	{
		c_hat.c[i][0] = y.c[i][0];
		for (int j = c_hat.lr - 1; j > i; j--)
		{
			temp = R.c[i][j] * c_hat.c[j][0];
			c_hat.c[i][0] = c_hat.c[i][0] - temp;
		}
		c_hat.c[i][0] = c_hat.c[i][0] / R.c[i][i];
		c_hat.c[i][0] = estimating_quantization_operation(c_hat.c[i][0]);
	}
	//�ͷ��ڴ�
	//H_temp.clear();	y.clear(); Q_H.clear(); R.clear();
	return c_hat;
}

//MMSE-SQRD�㷨
ComplexMatrix MMSE_SQRD(ComplexMatrix& x, ComplexMatrix& H, double sigma_n)
{
	//����x:nR * 1		//�ŵ�H:nR * nT		//��ԭ��c_hat:nT * 1
	ComplexMatrix c_hat(H.lc, 1, x.is_real);		//����Ļ�ԭ���ķ����ź�
	ComplexMatrix qi;								//��ȡQ����
	ComplexMatrix qk;								//��ȡQ����
	ComplexMatrix qiH;								//����Q���еĹ���ת��

	double q_2_norm_min;			//�洢Q��2-������С��һ�е�2-����
	int q_2_norm_min_column;		//�洢Q��2-������С��һ�еĴ洢�±�
	double r_ii;					//��ʱ�洢r_ii��ؼ���
	ComplexMatrix rik;				//��ʱ�洢r_ik��ؼ���

	ComplexMatrix temp;				//�����м����
															//QR�ֽ�������
	//��ʼ��QR�ֽ��R
	ComplexMatrix R(H.lc, H.lc, x.is_real);
	//��ʼ��QR�ֽ������QΪ����H
	ComplexMatrix Q(H);
	ComplexMatrix I0 = Q.make_eyes(H.lc);
	I0 = I0 * sigma_n;
	Q = Q.combine_rows(Q, I0);
	//��ʼ���洢������¼������
	int* p = new int[Q.lc];
	//��ʼ���洢Q���е�2-����������
	double* norms = new double[Q.lc];
	for (int i = 0; i < Q.lc; i++)
	{
		p[i] = i;
		norms[i] = 0.0;
	}
	for (int i = 0; i < Q.lc; i++)
	{
		qi = Q.get_column(i);
		norms[i] = qi.vector_2_norm();
		norms[i] *= norms[i];
	}
	for (int i = 0; i < Q.lc; i++)
	{
		//�ҳ�Qʣ������2-������С��һ���±�
		q_2_norm_min_column = i;
		q_2_norm_min = norms[i];
		for (int k = i + 1; k < Q.lc; k++)
			if (norms[k] < q_2_norm_min)
				q_2_norm_min_column = k;
		//������i�к͵�q_2_norm_min_column��
		R.exchange_column(i, q_2_norm_min_column);
		std::swap(p[i], p[q_2_norm_min_column]);
		std::swap(norms[i], norms[q_2_norm_min_column]);
		//��������Q�ĵ�i�к͵�q_2_norm_min_column�е�ǰ(nR+i-1)��
		Q.exchange_some_rows_of_column(i, q_2_norm_min_column, 0, x.lr + i - 1);
		//R�ĶԽ�Ԫ
		R.c[i][i] = sqrt(norms[i]);
		//Q���б�׼��
		r_ii = R.c[i][i].re;
		for (int j = 0; j < Q.lr; j++)
			Q.c[j][i] = Q.c[j][i] / r_ii;
		qi = Q.get_column(i);
		qiH = !qi;
		//�Ӻ����м�ȥ��������������
		for (int k = i + 1; k < Q.lc; k++)
		{
			qk = Q.get_column(k);
			//R�ķǶԽ�������Ԫ
			rik = qiH * qk;
			R.c[i][k] = rik.c[0][0];
			//����Q�ĵ�k��
			temp = rik.c[0][0] * qi;
			qk = qk - temp;
			for (int j = 0; j < Q.lr; j++)
				Q.c[j][k] = qk.c[j][0];
			//����norms�ĵ�k��
			norms[k] -= pow(rik.c[0][0].modulus(), 2);
		}
	}

	//ȡ��Q1��x����Q�任Ϊy
	Q = Q.get_rows(0, x.lr - 1);
	ComplexMatrix Q_H = !Q;
	ComplexMatrix y = Q_H * x;
	//����R��nT�к�y��ԭ�������ź�
	Complex temp1;						//�����м����
	for (int i = c_hat.lr - 1; i >= 0; i--)
	{
		c_hat.c[i][0] = y.c[i][0];
		for (int j = c_hat.lr - 1; j > i; j--)
		{
			temp1 = R.c[i][j] * c_hat.c[j][0];
			c_hat.c[i][0] = c_hat.c[i][0] - temp1;
		}
		c_hat.c[i][0] = c_hat.c[i][0] / R.c[i][i];
		c_hat.c[i][0] = estimating_quantization_operation(c_hat.c[i][0]);
	}
	//��S�л�ԭ�����ź�c��ԭ˳���ȸ���һ�ݣ��ٰ�S˳�򽫸���Ʒ�е�Ԫ����������������
	//��������ƣ���ʵ��������з�Χ�ڲ���Խ�磩
	ComplexMatrix c_hat_temp(c_hat);
	for (int i = 0; i < Q.lc; i++) c_hat.c[p[i]][0] = c_hat_temp.c[i][0];
	//�ͷ��ڴ�
	delete[]p; delete[]norms;
	return c_hat;
}

//MMSE-SQRD-PSA�㷨
ComplexMatrix MMSE_SQRD_PSA(ComplexMatrix& x, ComplexMatrix& H, double sigma_n)
{
	//����x:nR * 1		//�ŵ�H:nR * nT		//��ԭ��c_hat:nT * 1
	ComplexMatrix c_hat(H.lc, 1, x.is_real);		//����Ļ�ԭ���ķ����ź�
	ComplexMatrix qi;								//��ȡQ����
	ComplexMatrix qk;								//��ȡQ����
	ComplexMatrix qiH;								//����Q���еĹ���ת��

	double q_2_norm_min;			//�洢Q��2-������С��һ�е�2-����
	int q_2_norm_min_column;		//�洢Q��2-������С��һ�еĴ洢�±�
	double r_ii;					//��ʱ�洢r_ii��ؼ���
	ComplexMatrix rik;				//��ʱ�洢r_ik��ؼ���

	ComplexMatrix temp;				//�����м����
															//QR�ֽ�������
	//��ʼ��QR�ֽ��R
	ComplexMatrix R(H.lc, H.lc, x.is_real);
	//��ʼ��QR�ֽ������QΪ����H
	ComplexMatrix Q(H);
	ComplexMatrix I0 = Q.make_eyes(H.lc);
	I0 = I0 * sigma_n;
	Q = Q.combine_rows(Q, I0);
	//��ʼ���洢������¼������
	int* p = new int[Q.lc];
	//��ʼ���洢Q���е�2-����������
	double* norms = new double[Q.lc];
	for (int i = 0; i < Q.lc; i++)
	{
		p[i] = i;
		norms[i] = 0.0;
	}
	for (int i = 0; i < Q.lc; i++)
	{
		qi = Q.get_column(i);
		norms[i] = qi.vector_2_norm();
		norms[i] *= norms[i];
	}
	for (int i = 0; i < Q.lc; i++)
	{
		//�ҳ�Qʣ������2-������С��һ���±�
		q_2_norm_min_column = i;
		q_2_norm_min = norms[i];
		for (int k = i + 1; k < Q.lc; k++)
			if (norms[k] < q_2_norm_min)
				q_2_norm_min_column = k;
		//������i�к͵�q_2_norm_min_column��
		R.exchange_column(i, q_2_norm_min_column);
		std::swap(p[i], p[q_2_norm_min_column]);
		std::swap(norms[i], norms[q_2_norm_min_column]);
		//��������Q�ĵ�i�к͵�q_2_norm_min_column�е�ǰ(nR+i-1)��
		Q.exchange_some_rows_of_column(i, q_2_norm_min_column, 0, x.lr + i - 1);
		//R�ĶԽ�Ԫ
		R.c[i][i] = sqrt(norms[i]);
		//Q���б�׼��
		r_ii = R.c[i][i].re;
		for (int j = 0; j < Q.lr; j++)
			Q.c[j][i] = Q.c[j][i] / r_ii;
		qi = Q.get_column(i);
		qiH = !qi;
		//�Ӻ����м�ȥ��������������
		for (int k = i + 1; k < Q.lc; k++)
		{
			qk = Q.get_column(k);
			//R�ķǶԽ�������Ԫ
			rik = qiH * qk;
			R.c[i][k] = rik.c[0][0];
			//����Q�ĵ�k��
			temp = rik.c[0][0] * qi;
			qk = qk - temp;
			for (int j = 0; j < Q.lr; j++)
				Q.c[j][k] = qk.c[j][0];
			//����norms�ĵ�k��
			norms[k] -= pow(rik.c[0][0].modulus(), 2);
		}
	}

	//*********************************PSA*********************************
	ComplexMatrix Q1 = Q.get_rows(0, x.lr - 1);		//ȡ��Q1
	ComplexMatrix Q2 = Q.get_rows(x.lr, Q.lr - 1);	//ȡ��Q2
	ComplexMatrix Householder;						//����Householder�任��
	ComplexMatrix a, u, aH, uH, w1, w2;				//����Householder�任����м����
	int k_min = H.lc;								//Ѱ�������С��
	int ki = 0;										//Ѱ�������С��
	double* error_l = new double[H.lc];				//�������error

	for (int i = H.lc - 1; i >= 1; i--)
	{
		//Q2ÿ�����������ǵĲ��ֵ�2-������Ϊ���
		for (int l = 0; l <= i; l++)
		{
			temp = Q2.get_sub_matrix(l, l, 0, i);
			error_l[l] = pow(temp.vector_2_norm(), 2);
		}
		//�ҵ����������С�зŵ���ǰ�Ӿ������
		ki = i;
		for (int k = i - 1; k >= 0; k--)
			if (error_l[k] < error_l[ki])
				ki = k;
		if (k_min > ki)k_min = ki;
		//��������С�в��ڵ�ǰ�Ӿ������һ�У������󽻻�������Householder�����Q2���������
		if (ki < i)
		{
			Q2.exchange_row(i, ki);
			std::swap(p[i], p[ki]);
		}
		if (k_min < i)
		{
			//����Q2�ĵ�i�еĵ�k_min����(i-1)�е�Householder�任��
			a = Q2.get_sub_matrix(i, i, k_min, i);
			ComplexMatrix en(1, a.lc, a.is_real);
			en.c[0][en.lc - 1].re = a.vector_2_norm();
			u = a - en;
			double temp1 = 1 / u.vector_2_norm();
			u = u * temp1;
			uH = !u;
			aH = !a;
			w1 = u * aH;
			w2 = a * uH;
			Complex w = w1.c[0][0] / w2.c[0][0];
			w.re += 1.0;
			temp = uH * u;
			temp = w * temp;
			Householder = Householder.make_eyes(a.lc);
			Householder = Householder - temp;
			//Q2�ĵ�0��i�еĵ�k_min����(i-1)���ҳ�Householder�任��
			//Q1�ĵ�k_min����(i-1)���ҳ�Householder�任��
			//�ȷֱ��ڳ��Ӿ���ƴ�����ҳˣ������ȥ
			ComplexMatrix Q1__ = Q1.get_sub_matrix(0, Q1.lr - 1, k_min, i);
			ComplexMatrix Q2__ = Q2.get_sub_matrix(0, i, k_min, i);
			temp = temp.combine_rows(Q1__, Q2__);
			temp = temp * Householder;
			for (int jj = 0; jj < temp.lc; jj++)
			{
				for (int ii = 0; ii < Q1.lr; ii++)
					Q1.c[ii][jj + k_min] = temp.c[ii][jj];
				for (int ii = 0; ii <= i; ii++)
					Q2.c[ii][jj + k_min] = temp.c[ii + Q1.lr][jj];
			}
		}
	}
	double temp2 = 1 / sigma_n;
	ComplexMatrix Q2_inverse = Q2.square_inverse();
	R = temp2 * Q2_inverse;

	//*********************************��ԭ�ź�*****************************
	//x����Q�任Ϊy
	ComplexMatrix Q1_H = !Q1;
	ComplexMatrix y = Q1_H * x;
	//����R��nT�к�y��ԭ�������ź�
	Complex temp1;						//�����м����
	for (int i = c_hat.lr - 1; i >= 0; i--)
	{
		c_hat.c[i][0] = y.c[i][0];
		for (int j = c_hat.lr - 1; j > i; j--)
		{
			temp1 = R.c[i][j] * c_hat.c[j][0];
			c_hat.c[i][0] = c_hat.c[i][0] - temp1;
		}
		c_hat.c[i][0] = c_hat.c[i][0] / R.c[i][i];
		c_hat.c[i][0] = estimating_quantization_operation(c_hat.c[i][0]);
	}
	//��S�л�ԭ�����ź�c��ԭ˳���ȸ���һ�ݣ��ٰ�S˳�򽫸���Ʒ�е�Ԫ����������������
	//��������ƣ���ʵ��������з�Χ�ڲ���Խ�磩
	ComplexMatrix c_hat_temp(c_hat);
	for (int i = 0; i < Q.lc; i++) c_hat.c[p[i]][0] = c_hat_temp.c[i][0];

	//�ͷ��ڴ�
	delete[]p; delete[]norms; delete[]error_l;
	return c_hat;
}

//************************************ͳ�Ʋ����ļ�������************************************

//�����ļ������
ofstream dataOut;
//�Ƚ��������������ó�������
double BER(int* bits, int* bits_hat, int nT)
{
	int error = 0;
	for (int i = 0; i < nT * 2; i++)
		if (bits[i] != bits_hat[i])
			error++;
	double BER = double(error) / (double(nT) * 2.0);
	return BER;
}

//���������ʲ��ԣ�ָ������ͽ�������������������Ƚ��գ����䣩������n��H�ŵ�
//ÿ������n��H��һ�������㣬���ı�����ȣ�������һ��same_SNR_n_H
//�洢�����ʲ���һ�����������ֱ��¼ÿ�ַ�������һ������Ⱥ�һ�ַ�����ȷ���÷����ڴ�������µ�ƽ��������
//����������ݶ������txt�ļ����Ա�MATLAB��ͼ����
//һ�������㣺ָ������ͽ���������������һ������ȣ����ʱȣ�
//n��H�Ը��������һ��one_H_nn������n��H��ÿ�ַ���ʱ��ƽ�������ʣ���ÿ�ַ����ڸõ��������BER
//һ���������һ���֣�ָ������ͽ�������������H��ά����Ϣ�����֣���ͬһ��H
//ÿ�ַ�����ͬ������nn�Σ�Դ����������������ͬ��������ÿ�ַ�����ƽ��������
void BER_versus_SNR(int nR, int nT)
{
	int n = 100;						//H����
	cout << "H������"; cin >> n;
	double SNR = 1.0;					//���빦�ʱ�
	double SNR_sqrt = sqrt(SNR);		//������ȱ�
	int nn = 100;						//���д���
	cout << "ÿ��H���д�����"; cin >> nn;

	string filename;						//����nR��nT������ǰ���������ļ�
	char nR_1[5] = { '_','n','R','_','\0' }, nR_2[16] = { 0 }, nT_1[4] = { 'n','T','_','\0' }, nT_2[16] = { 0 };
	_itoa(nR, nR_2, 10); _itoa(nT, nT_2, 10);
	filename = string(nT_1) + string(nT_2) + string(nR_1) + string(nR_2) + ".txt";
	dataOut.open(filename.c_str());

	//�����ŵ�
	ComplexMatrix** H_n = new ComplexMatrix * [n];
	for (int i = 0; i < n; i++)
		H_n[i] = new ComplexMatrix(generate_H(nR, nT));
	//���з���
	for (int k = 0; k < 10; k++)		//ǰʮ��������SNRÿ������1����С1��ʼ
	{
		cout << "��ʼ���е�" << k + 1 << "��������...\n";
		ComplexMatrix AVG_BER(1, 4, true);	//ƽ�������ʳ�ʼ��
		SNR = 1.0 + double(k);
		for (int j = 0; j < n; j++)
		{
			SNR_sqrt = sqrt(SNR);
			ComplexMatrix H_temp(*(H_n[j]));
			H_temp = H_temp * SNR_sqrt;
			ComplexMatrix BERs(1, 4, true);	//�������ַ�����һ��H�������ʣ���ʼ��
			for (int l = 0; l < nn; l++)
			{
				ComplexMatrix c_hat_0;			//����ֱ�ӳ�α��ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_1;			//����V-BLAST�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_2;			//����������QRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_3;			//����SQRD�ӽ����ź��л�ԭ�ķ����ź�
				int* bits_hat_0;				//����ֱ�ӳ�α��ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_1;				//����V-BLAST�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_2;				//����������QRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_3;				//����SQRD�ӻ�ԭ�źŷ�����ı�����
				int* bits = generate_bits(H_temp.lc);					//����Դ������
				ComplexMatrix c = generate_signal(bits, H_temp.lc);		//���������ź�
				ComplexMatrix niu = generate_noise(H_temp.lr);			//��������
				ComplexMatrix x = H_temp * c; x = x + niu;				//���������ź�
				//���ַ����ӽ����źŻ�ԭ���������������
				c_hat_0 = Pseudo_inverse(x, H_temp);	bits_hat_0 = signal_to_bits(c_hat_0);
				c_hat_1 = V_BLAST(x, H_temp);			bits_hat_1 = signal_to_bits(c_hat_1);
				c_hat_2 = QRD(x, H_temp);				bits_hat_2 = signal_to_bits(c_hat_2);
				c_hat_3 = SQRD(x, H_temp);				bits_hat_3 = signal_to_bits(c_hat_3);
				//�ĸ������������Դ�������Ƚϣ�����������
				BERs.c[0][0].re += BER(bits, bits_hat_0, H_temp.lc);
				BERs.c[0][1].re += BER(bits, bits_hat_1, H_temp.lc);
				BERs.c[0][2].re += BER(bits, bits_hat_2, H_temp.lc);
				BERs.c[0][3].re += BER(bits, bits_hat_3, H_temp.lc);
				//�ͷ��ڴ�
				delete[]bits;
				delete[]bits_hat_0; delete[]bits_hat_1;
				delete[]bits_hat_2; delete[]bits_hat_3;
				c.clear(); niu.clear(); x.clear();
				c_hat_0.clear(); c_hat_1.clear(); c_hat_2.clear(); c_hat_3.clear();
			}
			//�ĸ������ʸ���ƽ��ֵ
			for (int i = 0; i < 4; i++)BERs.c[0][i].re /= nn;
			for (int i = 0; i < 4; i++)AVG_BER.c[0][i].re += BERs.c[0][i].re;
			//�ͷ��ڴ�
			//H_temp.clear();
		}
		//��������ƽ��ֵ�����
		for (int i = 0; i < 4; i++)AVG_BER.c[0][i].re /= n;
		dataOut << SNR << " " << AVG_BER;
		//�ͷ��ڴ�
		//AVG_BER.clear();
	}
	for (int k = 0; k < 2; k++)			//��2��������SNRÿ������10�����20����,��Ϊ���ֲ��Ծ����ں������ַ�����Ϊ0
	{
		cout << "��ʼ���е�" << k + 11 << "��������...\n";
		ComplexMatrix AVG_BER(1, 4, true);	//ƽ�������ʳ�ʼ��
		SNR = 20.0 + double(k) * 10;
		for (int j = 0; j < n; j++)
		{
			SNR_sqrt = sqrt(SNR);
			ComplexMatrix H_temp(*(H_n[j]));
			H_temp = H_temp * SNR_sqrt;
			ComplexMatrix BERs(1, 4, true);	//�������ַ�����һ��H�������ʣ���ʼ��
			for (int l = 0; l < nn; l++)
			{
				ComplexMatrix c_hat_0;			//����ֱ�ӳ�α��ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_1;			//����V-BLAST�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_2;			//����������QRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_3;			//����SQRD�ӽ����ź��л�ԭ�ķ����ź�
				int* bits_hat_0;				//����ֱ�ӳ�α��ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_1;				//����V-BLAST�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_2;				//����������QRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_3;				//����SQRD�ӻ�ԭ�źŷ�����ı�����
				int* bits = generate_bits(H_temp.lc);					//����Դ������
				ComplexMatrix c = generate_signal(bits, H_temp.lc);		//���������ź�
				ComplexMatrix niu = generate_noise(H_temp.lr);			//��������
				ComplexMatrix x = H_temp * c; x = x + niu;				//���������ź�
				//���ַ����ӽ����źŻ�ԭ���������������
				c_hat_0 = Pseudo_inverse(x, H_temp);	bits_hat_0 = signal_to_bits(c_hat_0);
				c_hat_1 = V_BLAST(x, H_temp);			bits_hat_1 = signal_to_bits(c_hat_1);
				c_hat_2 = QRD(x, H_temp);				bits_hat_2 = signal_to_bits(c_hat_2);
				c_hat_3 = SQRD(x, H_temp);				bits_hat_3 = signal_to_bits(c_hat_3);
				//�ĸ������������Դ�������Ƚϣ�����������
				BERs.c[0][0].re += BER(bits, bits_hat_0, H_temp.lc);
				BERs.c[0][1].re += BER(bits, bits_hat_1, H_temp.lc);
				BERs.c[0][2].re += BER(bits, bits_hat_2, H_temp.lc);
				BERs.c[0][3].re += BER(bits, bits_hat_3, H_temp.lc);
				//�ͷ��ڴ�
				delete[]bits;
				delete[]bits_hat_0; delete[]bits_hat_1;
				delete[]bits_hat_2; delete[]bits_hat_3;
				c.clear(); niu.clear(); x.clear();
				c_hat_0.clear(); c_hat_1.clear(); c_hat_2.clear(); c_hat_3.clear();
			}
			//�ĸ������ʸ���ƽ��ֵ
			for (int i = 0; i < 4; i++)BERs.c[0][i].re /= nn;
			for (int i = 0; i < 4; i++)AVG_BER.c[0][i].re += BERs.c[0][i].re;
			//�ͷ��ڴ�
			//H_temp.clear();
		}
		//��������ƽ��ֵ�����
		for (int i = 0; i < 4; i++)AVG_BER.c[0][i].re /= n;
		dataOut << SNR << " " << AVG_BER;
		//�ͷ��ڴ�
		//AVG_BER.clear();
	}
}

//MMSE���������ʲ���
void MMSE_BER_versus_SNR(int nR, int nT)
{
	int n = 100;						//H����
	cout << "H������"; cin >> n;
	double SNR = 1.0;					//���빦�ʱ�
	double SNR_sqrt = sqrt(SNR);		//������ȱ�
	int nn = 100;						//���д���
	cout << "ÿ��H���д�����"; cin >> nn;

	string filename;						//����nR��nT������ǰ���������ļ�
	char nR_1[5] = { '_','n','R','_','\0' }, nR_2[16] = { 0 }, nT_1[4] = { 'n','T','_','\0' }, nT_2[16] = { 0 };
	_itoa(nR, nR_2, 10); _itoa(nT, nT_2, 10);
	filename = string(nT_1) + string(nT_2) + string(nR_1) + string(nR_2) + "_MMSE.txt";
	dataOut.open(filename.c_str());

	//�����ŵ�
	ComplexMatrix** H_n = new ComplexMatrix * [n];
	for (int i = 0; i < n; i++)
		H_n[i] = new ComplexMatrix(generate_H(nR, nT));
	//���з���
	for (int k = 0; k < 10; k++)		//ǰ10��������SNRÿ������1����С1��ʼ
	{
		cout << "��ʼ���е�" << k + 1 << "��������...\n";
		ComplexMatrix AVG_BER(1, 5, true);	//ƽ�������ʳ�ʼ��
		SNR = 1.0 + double(k);
		for (int j = 0; j < n; j++)
		{
			SNR_sqrt = sqrt(SNR);
			ComplexMatrix H_temp(*(H_n[j]));
			H_temp = H_temp * SNR_sqrt;
			ComplexMatrix BERs(1, 5, true);	//����ÿ�ַ�����һ��H�������ʣ���ʼ��
			for (int l = 0; l < nn; l++)
			{
				ComplexMatrix c_hat_0;			//����ֱ�ӳ�α��ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_1;			//����V-BLAST�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_2;			//����������QRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_3;			//����SQRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_4;			//����SQRD_PSA�ӽ����ź��л�ԭ�ķ����ź�
				int* bits_hat_0;				//����ֱ�ӳ�α��ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_1;				//����V-BLAST�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_2;				//����������QRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_3;				//����SQRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_4;				//����SQRD_PSA�ӻ�ԭ�źŷ�����ı�����
				int* bits = generate_bits(H_temp.lc);					//����Դ������
				ComplexMatrix c = generate_signal(bits, H_temp.lc);		//���������ź�
				ComplexMatrix niu = generate_noise(H_temp.lr);			//��������
				ComplexMatrix x = H_temp * c; x = x + niu;				//���������ź�
				//�ӽ����źŻ�ԭ���������������
				c_hat_0 = MMSE_Pseudo_inverse(x, H_temp);	bits_hat_0 = signal_to_bits(c_hat_0);
				c_hat_1 = MMSE_V_BLAST(x, H_temp);			bits_hat_1 = signal_to_bits(c_hat_1);
				c_hat_2 = MMSE_QRD(x, H_temp, 1.0);			bits_hat_2 = signal_to_bits(c_hat_2);
				c_hat_3 = MMSE_SQRD(x, H_temp, 1.0);		bits_hat_3 = signal_to_bits(c_hat_3);
				c_hat_4 = MMSE_SQRD_PSA(x, H_temp, 1.0);	bits_hat_4 = signal_to_bits(c_hat_4);
				//�����������Դ�������Ƚϣ�����������
				BERs.c[0][0].re += BER(bits, bits_hat_0, H_temp.lc);
				BERs.c[0][1].re += BER(bits, bits_hat_1, H_temp.lc);
				BERs.c[0][2].re += BER(bits, bits_hat_2, H_temp.lc);
				BERs.c[0][3].re += BER(bits, bits_hat_3, H_temp.lc);
				BERs.c[0][4].re += BER(bits, bits_hat_4, H_temp.lc);
				//�ͷ��ڴ�
				delete[]bits;		delete[]bits_hat_4;
				delete[]bits_hat_0; delete[]bits_hat_1;
				delete[]bits_hat_2; delete[]bits_hat_3;
			}
			//�����ʸ���ƽ��ֵ
			for (int i = 0; i < 5; i++)BERs.c[0][i].re /= nn;
			for (int i = 0; i < 5; i++)AVG_BER.c[0][i].re += BERs.c[0][i].re;
		}
		//��������ƽ��ֵ�����
		for (int i = 0; i < 5; i++)AVG_BER.c[0][i].re /= n;
		dataOut << SNR << " " << AVG_BER;
	}
	for (int k = 0; k < 9; k++)			//��9��������SNRÿ������10�����100����
	{
		cout << "��ʼ���е�" << k + 11 << "��������...\n";
		ComplexMatrix AVG_BER(1, 5, true);	//ƽ�������ʳ�ʼ��
		SNR = 20.0 + double(k) * 10;
		for (int j = 0; j < n; j++)
		{
			SNR_sqrt = sqrt(SNR);
			ComplexMatrix H_temp(*(H_n[j]));
			H_temp = H_temp * SNR_sqrt;
			ComplexMatrix BERs(1, 5, true);	//����ÿ�ַ�����һ��H�������ʣ���ʼ��
			for (int l = 0; l < nn; l++)
			{
				ComplexMatrix c_hat_0;			//����ֱ�ӳ�α��ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_1;			//����V-BLAST�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_2;			//����������QRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_3;			//����SQRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_4;			//����SQRD_PSA�ӽ����ź��л�ԭ�ķ����ź�
				int* bits_hat_0;				//����ֱ�ӳ�α��ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_1;				//����V-BLAST�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_2;				//����������QRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_3;				//����SQRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_4;				//����SQRD_PSA�ӻ�ԭ�źŷ�����ı�����
				int* bits = generate_bits(H_temp.lc);					//����Դ������
				ComplexMatrix c = generate_signal(bits, H_temp.lc);		//���������ź�
				ComplexMatrix niu = generate_noise(H_temp.lr);			//��������
				ComplexMatrix x = H_temp * c; x = x + niu;				//���������ź�
				//�ӽ����źŻ�ԭ���������������
				c_hat_0 = MMSE_Pseudo_inverse(x, H_temp);	bits_hat_0 = signal_to_bits(c_hat_0);
				c_hat_1 = MMSE_V_BLAST(x, H_temp);			bits_hat_1 = signal_to_bits(c_hat_1);
				c_hat_2 = MMSE_QRD(x, H_temp, 1.0);			bits_hat_2 = signal_to_bits(c_hat_2);
				c_hat_3 = MMSE_SQRD(x, H_temp, 1.0);		bits_hat_3 = signal_to_bits(c_hat_3);
				c_hat_4 = MMSE_SQRD_PSA(x, H_temp, 1.0);	bits_hat_4 = signal_to_bits(c_hat_4);
				//�����������Դ�������Ƚϣ�����������
				BERs.c[0][0].re += BER(bits, bits_hat_0, H_temp.lc);
				BERs.c[0][1].re += BER(bits, bits_hat_1, H_temp.lc);
				BERs.c[0][2].re += BER(bits, bits_hat_2, H_temp.lc);
				BERs.c[0][3].re += BER(bits, bits_hat_3, H_temp.lc);
				BERs.c[0][4].re += BER(bits, bits_hat_4, H_temp.lc);
				//�ͷ��ڴ�
				delete[]bits;		delete[]bits_hat_4;
				delete[]bits_hat_0; delete[]bits_hat_1;
				delete[]bits_hat_2; delete[]bits_hat_3;
			}
			//�����ʸ���ƽ��ֵ
			for (int i = 0; i < 5; i++)BERs.c[0][i].re /= nn;
			for (int i = 0; i < 5; i++)AVG_BER.c[0][i].re += BERs.c[0][i].re;
		}
		//��������ƽ��ֵ�����
		for (int i = 0; i < 5; i++)AVG_BER.c[0][i].re /= n;
		dataOut << SNR << " " << AVG_BER;
	}
}

//ZF��MMSE���������ʲ���
void ZF_MMSE_BER_versus_SNR(int nR, int nT)
{
	int n = 100;						//H����
	cout << "H������"; cin >> n;
	double SNR = 1.0;					//���빦�ʱ�
	double SNR_sqrt = sqrt(SNR);		//������ȱ�
	int nn = 100;						//���д���
	cout << "ÿ��H���д�����"; cin >> nn;

	string filename;						//����nR��nT������ǰ���������ļ�
	char nR_1[5] = { '_','n','R','_','\0' }, nR_2[16] = { 0 }, nT_1[4] = { 'n','T','_','\0' }, nT_2[16] = { 0 };
	_itoa(nR, nR_2, 10); _itoa(nT, nT_2, 10);
	filename = string(nT_1) + string(nT_2) + string(nR_1) + string(nR_2) + "_ZF_MMSE.txt";
	dataOut.open(filename.c_str());

	//�����ŵ�
	ComplexMatrix** H_n = new ComplexMatrix * [n];
	for (int i = 0; i < n; i++)
		H_n[i] = new ComplexMatrix(generate_H(nR, nT));
	//���з���
	for (int k = 0; k < 10; k++)		//ǰ10��������SNRÿ������1����С1��ʼ
	{
		cout << "��ʼ���е�" << k + 1 << "��������...\n";
		ComplexMatrix AVG_BER(1, 9, true);	//ƽ�������ʳ�ʼ��
		SNR = 1.0 + double(k);
		for (int j = 0; j < n; j++)
		{
			SNR_sqrt = sqrt(SNR);
			ComplexMatrix H_temp(*(H_n[j]));
			H_temp = H_temp * SNR_sqrt;
			ComplexMatrix BERs(1, 9, true);		//����ÿ�ַ�����һ��H�������ʣ���ʼ��
			for (int l = 0; l < nn; l++)
			{
				ComplexMatrix c_hat_0;			//����ֱ�ӳ�α��ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_1;			//����V-BLAST�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_2;			//����������QRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_3;			//����SQRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_4;			//����MMSE����(ֱ�ӳ�α��)�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_5;			//����MMSE-V-BLAST�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_6;			//����MMSE-������QRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_7;			//����MMSE-SQRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_8;			//����MMSE-SQRD_PSA�ӽ����ź��л�ԭ�ķ����ź�
				int* bits_hat_0;				//����ֱ�ӳ�α��ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_1;				//����V-BLAST�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_2;				//����������QRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_3;				//����SQRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_4;				//����MMSE����(ֱ�ӳ�α��)�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_5;				//����MMSE-V-BLAST�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_6;				//����MMSE-������QRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_7;				//����MMSE-SQRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_8;				//����MMSE-SQRD_PSA�ӻ�ԭ�źŷ�����ı�����
				int* bits = generate_bits(H_temp.lc);					//����Դ������
				ComplexMatrix c = generate_signal(bits, H_temp.lc);		//���������ź�
				ComplexMatrix niu = generate_noise(H_temp.lr);			//��������
				ComplexMatrix x = H_temp * c; x = x + niu;				//���������ź�
				//�ӽ����źŻ�ԭ���������������
				c_hat_0 = Pseudo_inverse(x, H_temp);		bits_hat_0 = signal_to_bits(c_hat_0);
				c_hat_1 = V_BLAST(x, H_temp);				bits_hat_1 = signal_to_bits(c_hat_1);
				c_hat_2 = QRD(x, H_temp);					bits_hat_2 = signal_to_bits(c_hat_2);
				c_hat_3 = SQRD(x, H_temp);					bits_hat_3 = signal_to_bits(c_hat_3);
				c_hat_4 = MMSE_Pseudo_inverse(x, H_temp);	bits_hat_4 = signal_to_bits(c_hat_4);
				c_hat_5 = MMSE_V_BLAST(x, H_temp);			bits_hat_5 = signal_to_bits(c_hat_5);
				c_hat_6 = MMSE_QRD(x, H_temp, 1.0);			bits_hat_6 = signal_to_bits(c_hat_6);
				c_hat_7 = MMSE_SQRD(x, H_temp, 1.0);		bits_hat_7 = signal_to_bits(c_hat_7);
				c_hat_8 = MMSE_SQRD_PSA(x, H_temp, 1.0);	bits_hat_8 = signal_to_bits(c_hat_8);
				//�����������Դ�������Ƚϣ�����������
				BERs.c[0][0].re += BER(bits, bits_hat_0, H_temp.lc);
				BERs.c[0][1].re += BER(bits, bits_hat_1, H_temp.lc);
				BERs.c[0][2].re += BER(bits, bits_hat_2, H_temp.lc);
				BERs.c[0][3].re += BER(bits, bits_hat_3, H_temp.lc);
				BERs.c[0][4].re += BER(bits, bits_hat_4, H_temp.lc);
				BERs.c[0][5].re += BER(bits, bits_hat_5, H_temp.lc);
				BERs.c[0][6].re += BER(bits, bits_hat_6, H_temp.lc);
				BERs.c[0][7].re += BER(bits, bits_hat_7, H_temp.lc);
				BERs.c[0][8].re += BER(bits, bits_hat_8, H_temp.lc);
				//�ͷ��ڴ�
				delete[]bits;		delete[]bits_hat_0;
				delete[]bits_hat_1; delete[]bits_hat_2;
				delete[]bits_hat_3; delete[]bits_hat_4;
				delete[]bits_hat_5; delete[]bits_hat_6;
				delete[]bits_hat_7; delete[]bits_hat_8;
			}
			//�����ʸ���ƽ��ֵ
			for (int i = 0; i < 9; i++)BERs.c[0][i].re /= nn;
			for (int i = 0; i < 9; i++)AVG_BER.c[0][i].re += BERs.c[0][i].re;
		}
		//��������ƽ��ֵ�����
		for (int i = 0; i < 9; i++)AVG_BER.c[0][i].re /= n;
		dataOut << SNR << " " << AVG_BER;
	}
	for (int k = 0; k < 9; k++)			//��9��������SNRÿ������10�����100����
	{
		cout << "��ʼ���е�" << k + 11 << "��������...\n";
		ComplexMatrix AVG_BER(1, 9, true);	//ƽ�������ʳ�ʼ��
		SNR = 20.0 + double(k) * 10;
		for (int j = 0; j < n; j++)
		{
			SNR_sqrt = sqrt(SNR);
			ComplexMatrix H_temp(*(H_n[j]));
			H_temp = H_temp * SNR_sqrt;
			ComplexMatrix BERs(1, 9, true);		//����ÿ�ַ�����һ��H�������ʣ���ʼ��
			for (int l = 0; l < nn; l++)
			{
				ComplexMatrix c_hat_0;			//����ֱ�ӳ�α��ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_1;			//����V-BLAST�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_2;			//����������QRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_3;			//����SQRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_4;			//����MMSE����(ֱ�ӳ�α��)�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_5;			//����MMSE-V-BLAST�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_6;			//����MMSE-������QRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_7;			//����MMSE-SQRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_8;			//����MMSE-SQRD_PSA�ӽ����ź��л�ԭ�ķ����ź�
				int* bits_hat_0;				//����ֱ�ӳ�α��ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_1;				//����V-BLAST�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_2;				//����������QRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_3;				//����SQRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_4;				//����MMSE����(ֱ�ӳ�α��)�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_5;				//����MMSE-V-BLAST�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_6;				//����MMSE-������QRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_7;				//����MMSE-SQRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_8;				//����MMSE-SQRD_PSA�ӻ�ԭ�źŷ�����ı�����
				int* bits = generate_bits(H_temp.lc);					//����Դ������
				ComplexMatrix c = generate_signal(bits, H_temp.lc);		//���������ź�
				ComplexMatrix niu = generate_noise(H_temp.lr);			//��������
				ComplexMatrix x = H_temp * c; x = x + niu;				//���������ź�
				//�ӽ����źŻ�ԭ���������������
				c_hat_0 = Pseudo_inverse(x, H_temp);		bits_hat_0 = signal_to_bits(c_hat_0);
				c_hat_1 = V_BLAST(x, H_temp);				bits_hat_1 = signal_to_bits(c_hat_1);
				c_hat_2 = QRD(x, H_temp);					bits_hat_2 = signal_to_bits(c_hat_2);
				c_hat_3 = SQRD(x, H_temp);					bits_hat_3 = signal_to_bits(c_hat_3);
				c_hat_4 = MMSE_Pseudo_inverse(x, H_temp);	bits_hat_4 = signal_to_bits(c_hat_4);
				c_hat_5 = MMSE_V_BLAST(x, H_temp);			bits_hat_5 = signal_to_bits(c_hat_5);
				c_hat_6 = MMSE_QRD(x, H_temp, 1.0);			bits_hat_6 = signal_to_bits(c_hat_6);
				c_hat_7 = MMSE_SQRD(x, H_temp, 1.0);		bits_hat_7 = signal_to_bits(c_hat_7);
				c_hat_8 = MMSE_SQRD_PSA(x, H_temp, 1.0);	bits_hat_8 = signal_to_bits(c_hat_8);
				//�����������Դ�������Ƚϣ�����������
				BERs.c[0][0].re += BER(bits, bits_hat_0, H_temp.lc);
				BERs.c[0][1].re += BER(bits, bits_hat_1, H_temp.lc);
				BERs.c[0][2].re += BER(bits, bits_hat_2, H_temp.lc);
				BERs.c[0][3].re += BER(bits, bits_hat_3, H_temp.lc);
				BERs.c[0][4].re += BER(bits, bits_hat_4, H_temp.lc);
				BERs.c[0][5].re += BER(bits, bits_hat_5, H_temp.lc);
				BERs.c[0][6].re += BER(bits, bits_hat_6, H_temp.lc);
				BERs.c[0][7].re += BER(bits, bits_hat_7, H_temp.lc);
				BERs.c[0][8].re += BER(bits, bits_hat_8, H_temp.lc);
				//�ͷ��ڴ�
				delete[]bits;		delete[]bits_hat_0;
				delete[]bits_hat_1; delete[]bits_hat_2;
				delete[]bits_hat_3; delete[]bits_hat_4;
				delete[]bits_hat_5; delete[]bits_hat_6;
				delete[]bits_hat_7; delete[]bits_hat_8;
			}
			//�����ʸ���ƽ��ֵ
			for (int i = 0; i < 9; i++)BERs.c[0][i].re /= nn;
			for (int i = 0; i < 9; i++)AVG_BER.c[0][i].re += BERs.c[0][i].re;
		}
		//��������ƽ��ֵ�����
		for (int i = 0; i < 9; i++)AVG_BER.c[0][i].re /= n;
		dataOut << SNR << " " << AVG_BER;
	}
}

//V-BLAST��MMSE-V-BLAST�ֲ������ʲ���
void BER_versus_SNR_per_layer(int nR, int nT)
{
	int n = 100;						//H����
	cout << "H������"; cin >> n;
	double SNR = 1.0;					//���빦�ʱ�
	double SNR_sqrt = sqrt(SNR);		//������ȱ�
	int nn = 100;						//���д���
	cout << "ÿ��H���д�����"; cin >> nn;

	string filename;						//����nR��nT������ǰ���������ļ�
	char nR_1[5] = { '_','n','R','_','\0' }, nR_2[16] = { 0 }, nT_1[4] = { 'n','T','_','\0' }, nT_2[16] = { 0 };
	_itoa(nR, nR_2, 10); _itoa(nT, nT_2, 10);
	filename = string(nT_1) + string(nT_2) + string(nR_1) + string(nR_2) + "_V-BLAST_layers.txt";
	dataOut.open(filename.c_str());

	//�����ŵ�
	ComplexMatrix** H_n = new ComplexMatrix * [n];
	for (int i = 0; i < n; i++)
		H_n[i] = new ComplexMatrix(generate_H(nR, nT));
	//���з���
	for (int k = 0; k < 10; k++)		//ǰ10��������SNRÿ������1����С1��ʼ
	{
		cout << "��ʼ���е�" << k + 1 << "��������...\n";
		ComplexMatrix AVG_BER1(1, 4, true);	//V-BLASTƽ���ֲ������ʳ�ʼ��
		ComplexMatrix AVG_BER2(1, 4, true);	//MMSE-V-BLASTƽ���ֲ������ʳ�ʼ��
		SNR = 1.0 + double(k);
		for (int j = 0; j < n; j++)
		{
			SNR_sqrt = sqrt(SNR);
			ComplexMatrix H_temp(*(H_n[j]));
			H_temp = H_temp * SNR_sqrt;
			for (int l = 0; l < nn; l++)
			{
				ComplexMatrix BER_layers1;		//����V-BLAST�ֲ�������
				ComplexMatrix BER_layers2;		//����MMSE-V-BLAST�ֲ�������
				int* bits = generate_bits(H_temp.lc);					//����Դ������
				ComplexMatrix c = generate_signal(bits, H_temp.lc);		//���������ź�
				ComplexMatrix niu = generate_noise(H_temp.lr);			//��������
				ComplexMatrix x = H_temp * c; x = x + niu;				//���������ź�
				//��ȡ�µķֲ�������
				BER_layers2 = MMSE_V_BLAST_layer(x, H_temp, bits);
				BER_layers1 = V_BLAST_layer(x, H_temp, bits);
				//����ƽ��������
				AVG_BER1 = AVG_BER1 + BER_layers1;
				AVG_BER2 = AVG_BER2 + BER_layers2;
				//�ͷ��ڴ�
				delete[]bits;
			}
		}
		//��������ƽ��ֵ�����
		for (int i = 0; i < AVG_BER1.lc; i++) AVG_BER1.c[0][i].re /= (double(n) * double(nn));
		for (int i = 0; i < AVG_BER2.lc; i++) AVG_BER2.c[0][i].re /= (double(n) * double(nn));
		ComplexMatrix AVG_BER = AVG_BER1.combine_columns(AVG_BER1, AVG_BER2);
		dataOut << SNR << " " << AVG_BER;
	}
	for (int k = 0; k < 9; k++)			//��Ÿ�������SNRÿ������10�����100����
	{
		cout << "��ʼ���е�" << k + 11 << "��������...\n";
		ComplexMatrix AVG_BER1(1, 4, true);	//V-BLASTƽ���ֲ������ʳ�ʼ��
		ComplexMatrix AVG_BER2(1, 4, true);	//MMSE-V-BLASTƽ���ֲ������ʳ�ʼ��
		SNR = 20.0 + double(k) * 10;
		for (int j = 0; j < n; j++)
		{
			SNR_sqrt = sqrt(SNR);
			ComplexMatrix H_temp(*(H_n[j]));
			H_temp = H_temp * SNR_sqrt;
			for (int l = 0; l < nn; l++)
			{
				ComplexMatrix BER_layers1;		//����V-BLAST�ֲ�������
				ComplexMatrix BER_layers2;		//����MMSE-V-BLAST�ֲ�������
				int* bits = generate_bits(H_temp.lc);					//����Դ������
				ComplexMatrix c = generate_signal(bits, H_temp.lc);		//���������ź�
				ComplexMatrix niu = generate_noise(H_temp.lr);			//��������
				ComplexMatrix x = H_temp * c; x = x + niu;				//���������ź�
				//��ȡ�µķֲ�������
				BER_layers2 = MMSE_V_BLAST_layer(x, H_temp, bits);
				BER_layers1 = V_BLAST_layer(x, H_temp, bits);
				//����ƽ��������
				AVG_BER1 = AVG_BER1 + BER_layers1;
				AVG_BER2 = AVG_BER2 + BER_layers2;
				//�ͷ��ڴ�
				delete[]bits;
			}
		}
		//��������ƽ��ֵ�����
		for (int i = 0; i < AVG_BER1.lc; i++) AVG_BER1.c[0][i].re /= (double(n) * double(nn));
		for (int i = 0; i < AVG_BER2.lc; i++) AVG_BER2.c[0][i].re /= (double(n) * double(nn));
		ComplexMatrix AVG_BER = AVG_BER1.combine_columns(AVG_BER1, AVG_BER2);
		dataOut << SNR << " " << AVG_BER;
	}
}

//MMSE������֡�ʲ���
void MMSE_FER_versus_SNR(int nR, int nT)
{
	int n = 100;						//H����
	cout << "H������"; cin >> n;
	double SNR = 1.0;					//���빦�ʱ�
	double SNR_sqrt = sqrt(SNR);		//������ȱ�
	int nn = 100;						//���д���
	cout << "ÿ��H���д�����"; cin >> nn;

	string filename;						//����nR��nT������ǰ���������ļ�
	char nR_1[5] = { '_','n','R','_','\0' }, nR_2[16] = { 0 }, nT_1[4] = { 'n','T','_','\0' }, nT_2[16] = { 0 };
	_itoa(nR, nR_2, 10); _itoa(nT, nT_2, 10);
	filename = string(nT_1) + string(nT_2) + string(nR_1) + string(nR_2) + "_MMSE_FER.txt";
	dataOut.open(filename.c_str());

	//�����ŵ�
	ComplexMatrix** H_n = new ComplexMatrix * [n];
	for (int i = 0; i < n; i++)
		H_n[i] = new ComplexMatrix(generate_H(nR, nT));
	//���з���
	for (int k = 0; k < 10; k++)		//ǰ10��������SNRÿ������1����С1��ʼ
	{
		cout << "��ʼ���е�" << k + 1 << "��������...\n";
		ComplexMatrix AVG_FER(1, 5, true);	//ƽ����֡�ʳ�ʼ��
		SNR = 1.0 + double(k);
		for (int j = 0; j < n; j++)
		{
			SNR_sqrt = sqrt(SNR);
			ComplexMatrix H_temp(*(H_n[j]));
			H_temp = H_temp * SNR_sqrt;
			ComplexMatrix FERs(1, 5, true);	//����ÿ�ַ�����һ��H�������ʣ���ʼ��
			for (int l = 0; l < nn; l++)
			{
				ComplexMatrix c_hat_0;			//����ֱ�ӳ�α��ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_1;			//����V-BLAST�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_2;			//����������QRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_3;			//����SQRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_4;			//����SQRD_PSA�ӽ����ź��л�ԭ�ķ����ź�
				int* bits_hat_0;				//����ֱ�ӳ�α��ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_1;				//����V-BLAST�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_2;				//����������QRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_3;				//����SQRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_4;				//����SQRD_PSA�ӻ�ԭ�źŷ�����ı�����
				int* bits = generate_bits(H_temp.lc);					//����Դ������
				ComplexMatrix c = generate_signal(bits, H_temp.lc);		//���������ź�
				ComplexMatrix niu = generate_noise(H_temp.lr);			//��������
				ComplexMatrix x = H_temp * c; x = x + niu;				//���������ź�
				//�ӽ����źŻ�ԭ���������������
				c_hat_0 = MMSE_Pseudo_inverse(x, H_temp);	bits_hat_0 = signal_to_bits(c_hat_0);
				c_hat_1 = MMSE_V_BLAST(x, H_temp);			bits_hat_1 = signal_to_bits(c_hat_1);
				c_hat_2 = MMSE_QRD(x, H_temp, 1.0);			bits_hat_2 = signal_to_bits(c_hat_2);
				c_hat_3 = MMSE_SQRD(x, H_temp, 1.0);		bits_hat_3 = signal_to_bits(c_hat_3);
				c_hat_4 = MMSE_SQRD_PSA(x, H_temp, 1.0);	bits_hat_4 = signal_to_bits(c_hat_4);
				//�����������Դ�������Ƚϣ�������֡��
				bool* Frame_error = new bool[5];
				for (int i = 0; i < 5; i++)Frame_error[i] = false;
				for (int i = 0; i < nT * 2; i++)
				{
					if (bits[i] != bits_hat_0[i]) Frame_error[0] = true;
					if (bits[i] != bits_hat_1[i]) Frame_error[1] = true;
					if (bits[i] != bits_hat_2[i]) Frame_error[2] = true;
					if (bits[i] != bits_hat_3[i]) Frame_error[3] = true;
					if (bits[i] != bits_hat_4[i]) Frame_error[4] = true;
				}
				for (int i = 0; i < 5; i++)
					if (Frame_error[i]) FERs.c[0][i].re += 1.0;
				//�ͷ��ڴ�
				delete[]bits;		delete[]Frame_error;
				delete[]bits_hat_0; delete[]bits_hat_1;
				delete[]bits_hat_2; delete[]bits_hat_3;
				delete[]bits_hat_4;
			}
			//��֡�ʸ���ƽ��ֵ
			for (int i = 0; i < 5; i++)FERs.c[0][i].re /= nn;
			for (int i = 0; i < 5; i++)AVG_FER.c[0][i].re += FERs.c[0][i].re;
		}
		//��֡����ƽ��ֵ�����
		for (int i = 0; i < 5; i++)AVG_FER.c[0][i].re /= n;
		dataOut << SNR << " " << AVG_FER;
	}
	for (int k = 0; k < 9; k++)			//��9��������SNRÿ������10�����100����
	{
		cout << "��ʼ���е�" << k + 11 << "��������...\n";
		ComplexMatrix AVG_FER(1, 5, true);	//ƽ����֡�ʳ�ʼ��
		SNR = 20.0 + double(k) * 10;
		for (int j = 0; j < n; j++)
		{
			SNR_sqrt = sqrt(SNR);
			ComplexMatrix H_temp(*(H_n[j]));
			H_temp = H_temp * SNR_sqrt;
			ComplexMatrix FERs(1, 5, true);	//����ÿ�ַ�����һ��H�������ʣ���ʼ��
			for (int l = 0; l < nn; l++)
			{
				ComplexMatrix c_hat_0;			//����ֱ�ӳ�α��ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_1;			//����V-BLAST�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_2;			//����������QRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_3;			//����SQRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_4;			//����SQRD_PSA�ӽ����ź��л�ԭ�ķ����ź�
				int* bits_hat_0;				//����ֱ�ӳ�α��ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_1;				//����V-BLAST�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_2;				//����������QRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_3;				//����SQRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_4;				//����SQRD_PSA�ӻ�ԭ�źŷ�����ı�����
				int* bits = generate_bits(H_temp.lc);					//����Դ������
				ComplexMatrix c = generate_signal(bits, H_temp.lc);		//���������ź�
				ComplexMatrix niu = generate_noise(H_temp.lr);			//��������
				ComplexMatrix x = H_temp * c; x = x + niu;				//���������ź�
				//�ӽ����źŻ�ԭ���������������
				c_hat_0 = MMSE_Pseudo_inverse(x, H_temp);	bits_hat_0 = signal_to_bits(c_hat_0);
				c_hat_1 = MMSE_V_BLAST(x, H_temp);			bits_hat_1 = signal_to_bits(c_hat_1);
				c_hat_2 = MMSE_QRD(x, H_temp, 1.0);			bits_hat_2 = signal_to_bits(c_hat_2);
				c_hat_3 = MMSE_SQRD(x, H_temp, 1.0);		bits_hat_3 = signal_to_bits(c_hat_3);
				c_hat_4 = MMSE_SQRD_PSA(x, H_temp, 1.0);	bits_hat_4 = signal_to_bits(c_hat_4);
				//�����������Դ�������Ƚϣ�������֡��
				bool* Frame_error = new bool[5];
				for (int i = 0; i < 5; i++)Frame_error[i] = false;
				for (int i = 0; i < nT * 2; i++)
				{
					if (bits[i] != bits_hat_0[i]) Frame_error[0] = true;
					if (bits[i] != bits_hat_1[i]) Frame_error[1] = true;
					if (bits[i] != bits_hat_2[i]) Frame_error[2] = true;
					if (bits[i] != bits_hat_3[i]) Frame_error[3] = true;
					if (bits[i] != bits_hat_4[i]) Frame_error[4] = true;
				}
				for (int i = 0; i < 5; i++)
					if (Frame_error[i]) FERs.c[0][i].re += 1.0;
				//�ͷ��ڴ�
				delete[]bits;		delete[]Frame_error;
				delete[]bits_hat_0; delete[]bits_hat_1;
				delete[]bits_hat_2; delete[]bits_hat_3;
				delete[]bits_hat_4;
			}
			//��֡�ʸ���ƽ��ֵ
			for (int i = 0; i < 5; i++)FERs.c[0][i].re /= nn;
			for (int i = 0; i < 5; i++)AVG_FER.c[0][i].re += FERs.c[0][i].re;
		}
		//��֡����ƽ��ֵ�����
		for (int i = 0; i < 5; i++)AVG_FER.c[0][i].re /= n;
		dataOut << SNR << " " << AVG_FER;
	}
	for (int k = 0; k < 9; k++)			//��9��������SNRÿ������100�����1000����
	{
		cout << "��ʼ���е�" << k + 20 << "��������...\n";
		ComplexMatrix AVG_FER(1, 5, true);	//ƽ����֡�ʳ�ʼ��
		SNR = 200.0 + double(k) * 100;
		for (int j = 0; j < n; j++)
		{
			SNR_sqrt = sqrt(SNR);
			ComplexMatrix H_temp(*(H_n[j]));
			H_temp = H_temp * SNR_sqrt;
			ComplexMatrix FERs(1, 5, true);	//����ÿ�ַ�����һ��H�������ʣ���ʼ��
			for (int l = 0; l < nn; l++)
			{
				ComplexMatrix c_hat_0;			//����ֱ�ӳ�α��ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_1;			//����V-BLAST�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_2;			//����������QRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_3;			//����SQRD�ӽ����ź��л�ԭ�ķ����ź�
				ComplexMatrix c_hat_4;			//����SQRD_PSA�ӽ����ź��л�ԭ�ķ����ź�
				int* bits_hat_0;				//����ֱ�ӳ�α��ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_1;				//����V-BLAST�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_2;				//����������QRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_3;				//����SQRD�ӻ�ԭ�źŷ�����ı�����
				int* bits_hat_4;				//����SQRD_PSA�ӻ�ԭ�źŷ�����ı�����
				int* bits = generate_bits(H_temp.lc);					//����Դ������
				ComplexMatrix c = generate_signal(bits, H_temp.lc);		//���������ź�
				ComplexMatrix niu = generate_noise(H_temp.lr);			//��������
				ComplexMatrix x = H_temp * c; x = x + niu;				//���������ź�
				//�ӽ����źŻ�ԭ���������������
				c_hat_0 = MMSE_Pseudo_inverse(x, H_temp);	bits_hat_0 = signal_to_bits(c_hat_0);
				c_hat_1 = MMSE_V_BLAST(x, H_temp);			bits_hat_1 = signal_to_bits(c_hat_1);
				c_hat_2 = MMSE_QRD(x, H_temp, 1.0);			bits_hat_2 = signal_to_bits(c_hat_2);
				c_hat_3 = MMSE_SQRD(x, H_temp, 1.0);		bits_hat_3 = signal_to_bits(c_hat_3);
				c_hat_4 = MMSE_SQRD_PSA(x, H_temp, 1.0);	bits_hat_4 = signal_to_bits(c_hat_4);
				//�����������Դ�������Ƚϣ�������֡��
				bool* Frame_error = new bool[5];
				for (int i = 0; i < 5; i++)Frame_error[i] = false;
				for (int i = 0; i < nT * 2; i++)
				{
					if (bits[i] != bits_hat_0[i]) Frame_error[0] = true;
					if (bits[i] != bits_hat_1[i]) Frame_error[1] = true;
					if (bits[i] != bits_hat_2[i]) Frame_error[2] = true;
					if (bits[i] != bits_hat_3[i]) Frame_error[3] = true;
					if (bits[i] != bits_hat_4[i]) Frame_error[4] = true;
				}
				for (int i = 0; i < 5; i++)
					if (Frame_error[i]) FERs.c[0][i].re += 1.0;
				//�ͷ��ڴ�
				delete[]bits;		delete[]Frame_error;
				delete[]bits_hat_0; delete[]bits_hat_1;
				delete[]bits_hat_2; delete[]bits_hat_3;
				delete[]bits_hat_4;
			}
			//��֡�ʸ���ƽ��ֵ
			for (int i = 0; i < 5; i++)FERs.c[0][i].re /= nn;
			for (int i = 0; i < 5; i++)AVG_FER.c[0][i].re += FERs.c[0][i].re;
		}
		//��֡����ƽ��ֵ�����
		for (int i = 0; i < 5; i++)AVG_FER.c[0][i].re /= n;
		dataOut << SNR << " " << AVG_FER;
	}
}

//************************************������************************************************

int main()
{
	srand(int(time(NULL)));	//�������������
	int nT, nR;
	
	//*******************************(1)�������1*******************************************
	nT = 4;		//����������
	nR = 4;		//����������
	cout << "(1)�������1\n";
	//����bits����ʾ
	int* bits = generate_bits(nT);
	cout << "������ ="; for (int i = 0; i < nT * 2; i++) cout << bits[i]; cout << endl;
	//���Ƴɷ����źţ������ŵ�������������ȫ����ʾ
	ComplexMatrix c = generate_signal(bits, nT);
	cout << "�����ź�c = \n" << c << endl;
	ComplexMatrix H = generate_H(nR, nT);
	cout << "�ŵ�����H = \n" << H << endl;
	ComplexMatrix niu = generate_noise(nR);
	cout << "������ = \n" << niu << endl;
	//���������ź�
	ComplexMatrix x = H * c;
	x = x + niu;
	//�����㷨���Իָ������ź�Ϊ�����źţ���ʾ���������ԭΪbits����ʾ���
	ComplexMatrix result = Pseudo_inverse(x, H);  int* result_bits = signal_to_bits(result);
	cout << "ֱ�ӳ�α���㷨��� = \n" << result << endl << "��������� =";
	for (int i = 0; i < nT * 2; i++) cout << result_bits[i]; cout << endl << endl;
	
	result = V_BLAST(x, H);  result_bits = signal_to_bits(result); 
	cout << "V-BLAST�㷨��� = \n" << result << endl << "��������� =";
	for (int i = 0; i < nT * 2; i++) cout << result_bits[i]; cout << endl << endl;

	result = QRD(x, H);	result_bits = signal_to_bits(result); 
	cout << "������QRD�㷨��� = \n" << result << endl << "��������� =";
	for (int i = 0; i < nT * 2; i++) cout << result_bits[i]; cout << endl << endl;

	result = SQRD(x, H); result_bits = signal_to_bits(result); 
	cout << "SQRD�㷨��� = \n" << result << endl << "��������� =";
	for (int i = 0; i < nT * 2; i++) cout << result_bits[i]; cout << endl << endl;

	result = MMSE_Pseudo_inverse(x, H); result_bits = signal_to_bits(result);
	cout << "MMSE_ֱ�ӳ�α���㷨��� = \n" << result << endl << "��������� =";
	for (int i = 0; i < nT * 2; i++) cout << result_bits[i]; cout << endl << endl;

	result = MMSE_V_BLAST(x, H); result_bits = signal_to_bits(result);
	cout << "MMSE_V-BLAST�㷨��� = \n" << result << endl << "��������� =";
	for (int i = 0; i < nT * 2; i++) cout << result_bits[i]; cout << endl << endl;

	result = MMSE_QRD(x, H, 1.0); result_bits = signal_to_bits(result);
	cout << "MMSE_QRD�㷨��� = \n" << result << endl << "��������� =";
	for (int i = 0; i < nT * 2; i++) cout << result_bits[i]; cout << endl << endl;

	result = MMSE_SQRD(x, H, 1.0); result_bits = signal_to_bits(result);
	cout << "MMSE_SQRD�㷨��� = \n" << result << endl << "��������� =";
	for (int i = 0; i < nT * 2; i++) cout << result_bits[i]; cout << endl << endl;

	result = MMSE_SQRD_PSA(x, H, 1.0); result_bits = signal_to_bits(result);
	cout << "MMSE_SQRD_PSA�㷨��� = \n" << result << endl << "��������� =";
	for (int i = 0; i < nT * 2; i++) cout << result_bits[i]; cout << endl << endl;

	//*******************************(2)���ؿ���������************************************
	int input;
	cout << "ѡ��\n2.ZF���������ʲ��ԣ�3.V-BLAST��MMSE-V-BLAST�ֲ������ʲ��ԣ�4.MMSE������֡�ʲ��ԣ�5.ZF��MMSE���������ʲ��ԣ�";
	cin >> input;
	cout << "�������������nR��"; cin >> nR;
	cout << "���뷢��������nT��"; cin >> nT;
	if (input == 2)
	{
		BER_versus_SNR(nR, nT);
		cout << "�������\n";
	}
	else if (input == 3)
	{
		BER_versus_SNR_per_layer(nR, nT);
		cout << "�������\n";
	}
	else if (input == 4)
	{
		MMSE_FER_versus_SNR(nR, nT);
		cout << "�������\n";
	}
	else if (input == 5)
	{
		ZF_MMSE_BER_versus_SNR(nR, nT);
		cout << "�������\n";
	}
	return 0;
}