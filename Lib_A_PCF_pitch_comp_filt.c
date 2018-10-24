/**
 * @file    PCF_pitch_compl_filt.c
 * @author  Kuroha
 * @version
 * @date    28 сентября 2018 г., 12:55
 * @brief
 */


/*#### |Begin| --> Секция - "Include" ########################################*/
#include "Lib_A_PCF_pitch_comp_filt.h"
/*#### |End  | <-- Секция - "Include" ########################################*/


/*#### |Begin| --> Секция - "Глобальные переменные" ##########################*/
/*#### |End  | <-- Секция - "Глобальные переменные" ##########################*/


/*#### |Begin| --> Секция - "Локальные переменные" ###########################*/
/*#### |End  | <-- Секция - "Локальные переменные" ###########################*/


/*#### |Begin| --> Секция - "Прототипы локальных функций" ####################*/
static __PCF_FPT__
RestrictionSaturation (
	__PCF_FPT__ value,
	__PCF_FPT__ saturation);
/*#### |End  | <-- Секция - "Прототипы локальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание глобальных функций" ####################*/

/**
 * @brief	Функция выполняет инициализацию структуры для расчета текущего угла
 * 			наклона с помощью комплиментарного фильтра
 * @param[in,out]	*p_s:   Указатель на структуру, в которой
 * 							содержится необходимая информация для
 * 							работы функции
 * @param[in]	*pInit_s:	Указатель на структуру, в которой содержится
 * 							необходимая информация для инициализации
 * 							структуры p_s
 * @return None
 */
void
PCF_Init_CompFilt(
	pcf_data_for_pitch_s *p_s,
	pcf_data_for_pitch_init_struct_s *pInit_s)
{
	p_s->angle	= 0.0f;
	p_s->err	= 0.0f;

	p_s->compFiltCoeff	= pInit_s->compFiltCoeff;
	p_s->dT				= pInit_s->dT;
	p_s->integralCoeff	= pInit_s->integralCoeff;
	p_s->accNormWindow	= pInit_s->accNormWindow;
	p_s->integralErrorSaturation	= pInit_s->integralErrorSaturation;

	size_t i = PCF_ROLL;
	for (i = PCF_ROLL; i <= PCF_YAW; i++)
	{
		p_s->compFiltForAcc_a[i].filtCoeff =
			pInit_s->compFiltValForAcc;
	}


	p_s->initPitchEn_flag = 1u;

	ninteg_trapz_init_struct_s trapzInit_s;
	NINTEG_Trapz_StructInit(
		&trapzInit_s);
	trapzInit_s.accumulate_flag = NINTEG_DISABLE;
	trapzInit_s.integratePeriod = pInit_s->dT;
	NINTEG_Trapz_Init(
		&p_s->trapzInteg_s,
		&trapzInit_s);
}

void
PCF_CompFilt_StructInit(
	pcf_data_for_pitch_init_struct_s *pInit_s)
{
	pInit_s->accNormWindow	= (__PCF_FPT__) 0.0;
	pInit_s->compFiltCoeff	= (__PCF_FPT__) 0.0;
	pInit_s->dT				= (__PCF_FPT__) 0.0;
	pInit_s->integralCoeff	= (__PCF_FPT__) 0.0;
	pInit_s->integralErrorSaturation = (__PCF_FPT__) 0.0;
}

/**
 * @brief	Функция вычисляет текущий угол наклона по тангажу с помощью
 *        	комплиментарного фильтр, а также обновляет данные в структуре
 *         	с необходимой информацией
 * @param[in,out]	*p_s:	Указатель на структуру, в которой
 * 							содержится необходимая информация для
 * 							работы функции
 * @param[in]	gyrY:	Текущее значение гироскопа по оси тангажа
 * @param[in]	accX:	Текущее значение акселерометра по оси направления
 *                      движения
 * @param[in]	accZ:	Текущее значение акселерометра по оси, направленной
 * 						по направлению вектора силы тяжести
 * @return Текущий угол наклона
 */
__PCF_FPT__
PCF_GetPitchByCompFilt(
	pcf_data_for_pitch_s *p_s,
	__PCF_FPT__ *gyrY,
	__PCF_FPT__ accX,
	__PCF_FPT__ accY,
	__PCF_FPT__ accZ)
{
	if (((*gyrY != NAN) && (*gyrY != 0.0))
			&& ((accX != NAN) && (accX != 0.0))
			&& ((accZ != NAN) && (accZ != 0.0)))
	{
		/* Если флаг инициализации угла наклона равен 1*/
		if (p_s->initPitchEn_flag == 1u)
		{
			p_s->angle =
				(__PCF_FPT__) __PCF_atan2(accX, accZ);

			/* Сброс Флага */
			p_s->initPitchEn_flag = 0u;
		}

		/* Определение нормы вектора акселерометра */
		p_s->accNorm = accX * accX + accY * accY + accZ * accZ;

		__PCF_FPT__ compFiltCoeff = p_s->compFiltCoeff;

		if ((p_s->accNorm > ((__PCF_FPT__) 1.0 + p_s->accNormWindow))
				|| p_s->accNorm < (__PCF_FPT__) 1.0 - p_s->accNormWindow)
		{
			compFiltCoeff = (__PCF_FPT__) 1.0;
		}

		/* Нормирование вектора показаний акселерометра */
		p_s->accNorm = __PCF_sqrt(p_s->accNorm);
		accX /= p_s->accNorm;
		accY /= p_s->accNorm;
		accZ /= p_s->accNorm;

		accX =
			FILT_Complementary_fpt(
				&p_s->compFiltForAcc_a[PCF_ROLL],
				accX);
		accY =
			FILT_Complementary_fpt(
				&p_s->compFiltForAcc_a[PCF_PITCH],
				accY);
		accZ =
			FILT_Complementary_fpt(
				&p_s->compFiltForAcc_a[PCF_YAW],
				accZ);

		/* Получить угол наклона по показаниям акселерометра */
		__PCF_FPT__ pitchByAcc =
			(__PCF_FPT__) __PCF_atan2(accX, accZ);
		__PCF_FPT__ angleAcc =
			pitchByAcc * (((__PCF_FPT__) 1.0) - compFiltCoeff);

		/* Компенсация gyro bias измеренного значения */
		*gyrY -= p_s->err;

		/* Найти приращение угла наклона за промежуток времени dT */
		__PCF_FPT__ deltaPitch =
			NINTEG_Trapz(
				&p_s->trapzInteg_s,
				*gyrY);

		/* Получить угол наклона по показаниям гироскопа */
		__PCF_FPT__ angleGyr = (p_s->angle + deltaPitch) * compFiltCoeff;

		p_s->angle = angleGyr + angleAcc;

		/* Интегральная коррекция ошибки */
		p_s->err += (p_s->angle - pitchByAcc) * p_s->integralCoeff;
		p_s->err =
			RestrictionSaturation(
				p_s->err,
				p_s->integralErrorSaturation);
	}
	return p_s->angle;
}

__PCF_FPT__
RestrictionSaturation (
	__PCF_FPT__ value,
	__PCF_FPT__ saturation)
{
	/*    Ограничение насыщения выходного параметра */
	//    Если переменная насыщения не равна "0.0":
	if (saturation != (__PCF_FPT__) 0.0f)
	{
		//    Если выходное значение больше положительного значения переменной насыщения:
		if (value > saturation)
		{
			value = saturation;
		}
		//  Если выходное значение меньше отрицательного значения переменной насыщения:
		else if (value < (-saturation))
		{
			value = -saturation;
		}
	}
	return value;
}
/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
