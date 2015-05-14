#ifndef SIGNATURE_H_INCLUDED
#define SIGNATURE_H_INCLUDED
/**
 * @defgroup ǩ����ӡ�ӿں���
 * @author �ܽ�
 * @brief �ṩǩ���ĳ�ʼ���͹رպ����Լ���ǩ�����д�ӡ
 * @{
 */

/**
 * @fn void Os__clear_sign(void)
 * @brief 	���ǩ������
 *
 * @param �޲���
 *
 * @return 	�޷���ֵ
 *
 */
void Os__clear_sign(void);

/**
 * @fn void Os__close_sign(void)
 * @brief 	�ر�ǩ����ʼ��ʱ�򿪵Ľڵ�Ϳ��ٵ��ڴ�ռ�,
 * 			ʹOs__get_sign()�����˳���ѭ��
 *
 * @param �޲���
 *
 * @return 	�޷���ֵ
 *
 */
void Os__close_sign(void);

/**
 * @fn void Os__get_sign(void)
 * @brief 	��ȡǩ���Ĺ켣,��Ҫ�����߳���,
 * 			�˺�����һ����ѭ��ֱ������Os__close_sign()Ϊֹ
 *
 * @param �޲���
 *
 * @return 	�޷���ֵ
 *
 */
void Os__get_sign(void);

/**
 * @fn int Os__init_sign(int x,int y,int width,int height)
 * @brief 	��ʼ��ǩ��������ǩ������
 *
 * @param x			ǩ�������x����
 * @param y			ǩ�������y����
 * @param width		ǩ������Ŀ��
 * @param height	ǩ������ĸ߶�
 *
 * @return 		�ɹ�����0,ʧ�ܷ���-1
 *
 */
int Os__init_sign(int x,int y,int width,int height);

/**
 * @fn int Os__print_sign(void)
 * @brief 	��ӡǩ��
 *
 * @param �޲���
 *
 * @code
 * int main(void)
 * {
 * 		if(Os__init_sign(0,50,240,211) == 0)
 * 		{
 * 			//�˴�Ӧ��ʹ���߳������ô˺���
 * 			Os__get_sign();
 * 			//�ر�
 * 			Os__close_sign();
 * 		}
 * 		return 0;
 * }
 * @endcode
 * @return 		�ɹ�����0,��ֽ����-2,ʧ�ܷ���-1
 *
 */
int Os__print_sign(void);
#endif  //SIGNATURE_H_INCLUDED
/** @} end of etc_fns */