/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-3-30
  * ��    ��: ���ض��������ײ���������
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F4Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "key/bsp_key.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
__IO uint8_t flag_nfc_status=0; 

/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ���ذ���IO���ų�ʼ��.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����ʹ�ú궨�巽������������źţ����������ֲ��ֻҪ���޸�bsp_key.h
  *           �ļ���غ궨��Ϳ��Է����޸����š�
  */
void KEY_GPIO_Init(void)
{
   /* ����IOӲ����ʼ���ṹ����� */
  GPIO_InitTypeDef GPIO_InitStruct;
	
	/* ʹ��(����)KEY���Ŷ�ӦIO�˿�ʱ�� */  
  KEY1_RCC_CLK_ENABLE();
  KEY2_RCC_CLK_ENABLE(); 
  KEY3_RCC_CLK_ENABLE();
  KEY4_RCC_CLK_ENABLE();	
  KEY5_RCC_CLK_ENABLE(); 
  
  /* ����KEY1 GPIO:�ⲿ���ж��½�����Чģʽ */
  GPIO_InitStruct.Pin = KEY1_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY1_GPIO, &GPIO_InitStruct);  
  
  /* ����KEY2 GPIO:�ⲿ���ж��½�����Чģʽ */
  GPIO_InitStruct.Pin = KEY2_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY2_GPIO, &GPIO_InitStruct);  

  /* ����KEY3 GPIO:�ⲿ���ж��½�����Чģʽ */
  GPIO_InitStruct.Pin = KEY3_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY3_GPIO, &GPIO_InitStruct);
  
  /* ����KEY4 GPIO:�ⲿ���ж��½�����Чģʽ */
  GPIO_InitStruct.Pin = KEY4_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY4_GPIO, &GPIO_InitStruct); 
  
  /* ����KEY5 GPIO:�ⲿ���ж��½�����Чģʽ */
  GPIO_InitStruct.Pin = KEY5_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY5_GPIO, &GPIO_InitStruct);  
  
  /* �����ж����ȼ� */
  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 1, 2);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 1);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);  
}

/**
  * ��������: ��ȡ����KEY1��״̬
  * �����������
  * �� �� ֵ: KEY_DOWN�����������£�
  *           KEY_UP  ������û������
  * ˵    �����ޡ�
  */
KEYState_TypeDef KEY1_StateRead(void)
{
  /* ��ȡ��ʱ����ֵ���ж��Ƿ��Ǳ�����״̬������Ǳ�����״̬���뺯���� */
  if(HAL_GPIO_ReadPin(KEY1_GPIO,KEY1_GPIO_PIN)==KEY1_DOWN_LEVEL)
  {
    /* ��ʱһС��ʱ�䣬�������� */
    HAL_Delay(10);
    /* ��ʱʱ��������жϰ���״̬��������ǰ���״̬˵������ȷʵ������ */
    if(HAL_GPIO_ReadPin(KEY1_GPIO,KEY1_GPIO_PIN)==KEY1_DOWN_LEVEL)
    {
      /* �ȴ������������˳�����ɨ�躯�� */
      while(HAL_GPIO_ReadPin(KEY1_GPIO,KEY1_GPIO_PIN)==KEY1_DOWN_LEVEL);
       /* ����ɨ����ϣ�ȷ�����������£����ذ���������״̬ */
      return KEY_DOWN;
    }
  }
  /* ����û�����£�����û������״̬ */
  return KEY_UP;
}

/**
  * ��������: ��ȡ����KEY2��״̬
  * �����������
  * �� �� ֵ: KEY_DOWN�����������£�
  *           KEY_UP  ������û������
  * ˵    �����ޡ�
  */
KEYState_TypeDef KEY2_StateRead(void)
{
  /* ��ȡ��ʱ����ֵ���ж��Ƿ��Ǳ�����״̬������Ǳ�����״̬���뺯���� */
  if(HAL_GPIO_ReadPin(KEY2_GPIO,KEY2_GPIO_PIN)==KEY2_DOWN_LEVEL)
  {
    /* ��ʱһС��ʱ�䣬�������� */
    HAL_Delay(10);
    /* ��ʱʱ��������жϰ���״̬��������ǰ���״̬˵������ȷʵ������ */
    if(HAL_GPIO_ReadPin(KEY2_GPIO,KEY2_GPIO_PIN)==KEY2_DOWN_LEVEL)
    {
      /* �ȴ������������˳�����ɨ�躯�� */
      while(HAL_GPIO_ReadPin(KEY2_GPIO,KEY2_GPIO_PIN)==KEY2_DOWN_LEVEL);
       /* ����ɨ����ϣ�ȷ�����������£����ذ���������״̬ */
      return KEY_DOWN;
    }
  }
  /* ����û�����£�����û������״̬ */
  return KEY_UP;
}

/**
  * ��������: ��ȡ����KEY3��״̬
  * �����������
  * �� �� ֵ: KEY_DOWN�����������£�
  *           KEY_UP  ������û������
  * ˵    �����ޡ�
  */
KEYState_TypeDef KEY3_StateRead(void)
{
  /* ��ȡ��ʱ����ֵ���ж��Ƿ��Ǳ�����״̬������Ǳ�����״̬���뺯���� */
  if(HAL_GPIO_ReadPin(KEY3_GPIO,KEY3_GPIO_PIN)==KEY3_DOWN_LEVEL)
  {
    /* ��ʱһС��ʱ�䣬�������� */
    HAL_Delay(10);
    /* ��ʱʱ��������жϰ���״̬��������ǰ���״̬˵������ȷʵ������ */
    if(HAL_GPIO_ReadPin(KEY3_GPIO,KEY3_GPIO_PIN)==KEY3_DOWN_LEVEL)
    {
      /* �ȴ������������˳�����ɨ�躯�� */
      while(HAL_GPIO_ReadPin(KEY3_GPIO,KEY3_GPIO_PIN)==KEY3_DOWN_LEVEL);
       /* ����ɨ����ϣ�ȷ�����������£����ذ���������״̬ */
      return KEY_DOWN;
    }
  }
  /* ����û�����£�����û������״̬ */
  return KEY_UP;
}

/**
  * ��������: ��ȡ����KEY4��״̬
  * �����������
  * �� �� ֵ: KEY_DOWN�����������£�
  *           KEY_UP  ������û������
  * ˵    �����ޡ�
  */
KEYState_TypeDef KEY4_StateRead(void)
{
  /* ��ȡ��ʱ����ֵ���ж��Ƿ��Ǳ�����״̬������Ǳ�����״̬���뺯���� */
  if(HAL_GPIO_ReadPin(KEY4_GPIO,KEY4_GPIO_PIN)==KEY4_DOWN_LEVEL)
  {
    /* ��ʱһС��ʱ�䣬�������� */
    HAL_Delay(10);
    /* ��ʱʱ��������жϰ���״̬��������ǰ���״̬˵������ȷʵ������ */
    if(HAL_GPIO_ReadPin(KEY4_GPIO,KEY4_GPIO_PIN)==KEY4_DOWN_LEVEL)
    {
      /* �ȴ������������˳�����ɨ�躯�� */
      while(HAL_GPIO_ReadPin(KEY4_GPIO,KEY4_GPIO_PIN)==KEY4_DOWN_LEVEL);
       /* ����ɨ����ϣ�ȷ�����������£����ذ���������״̬ */
      return KEY_DOWN;
    }
  }
  /* ����û�����£�����û������״̬ */
  return KEY_UP;
}

/**
  * ��������: ��ȡ����KEY5��״̬
  * �����������
  * �� �� ֵ: KEY_DOWN�����������£�
  *           KEY_UP  ������û������
  * ˵    �����ޡ�
  */
KEYState_TypeDef KEY5_StateRead(void)
{
  /* ��ȡ��ʱ����ֵ���ж��Ƿ��Ǳ�����״̬������Ǳ�����״̬���뺯���� */
  if(HAL_GPIO_ReadPin(KEY5_GPIO,KEY5_GPIO_PIN)==KEY5_DOWN_LEVEL)
  {
    /* ��ʱһС��ʱ�䣬�������� */
    HAL_Delay(10);
    /* ��ʱʱ��������жϰ���״̬��������ǰ���״̬˵������ȷʵ������ */
    if(HAL_GPIO_ReadPin(KEY5_GPIO,KEY5_GPIO_PIN)==KEY5_DOWN_LEVEL)
    {
      /* �ȴ������������˳�����ɨ�躯�� */
      while(HAL_GPIO_ReadPin(KEY5_GPIO,KEY5_GPIO_PIN)==KEY5_DOWN_LEVEL);
       /* ����ɨ����ϣ�ȷ�����������£����ذ���������״̬ */
      return KEY_DOWN;
    }
  }
  /* ����û�����£�����û������״̬ */
  return KEY_UP;
}

/**
  * ��������: �����ⲿ�жϷ�����
  * �������: GPIO_Pin���ж�����
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin==KEY1_GPIO_PIN)
  {
    HAL_Delay(20);/* ��ʱһС��ʱ�䣬�������� */
    if(HAL_GPIO_ReadPin(KEY1_GPIO,KEY1_GPIO_PIN)==KEY1_DOWN_LEVEL)
    {
      flag_nfc_status=1;
    }
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_GPIO_PIN);
  }
  else if(GPIO_Pin==KEY2_GPIO_PIN)
  {
    HAL_Delay(20);/* ��ʱһС��ʱ�䣬�������� */
    if(HAL_GPIO_ReadPin(KEY2_GPIO,KEY2_GPIO_PIN)==KEY2_DOWN_LEVEL)
    {
      flag_nfc_status=2;
    }
    __HAL_GPIO_EXTI_CLEAR_IT(KEY2_GPIO_PIN);
  }
  else if(GPIO_Pin==KEY3_GPIO_PIN)
  {
    HAL_Delay(20);/* ��ʱһС��ʱ�䣬�������� */
    if(HAL_GPIO_ReadPin(KEY3_GPIO,KEY3_GPIO_PIN)==KEY3_DOWN_LEVEL)
    {
      flag_nfc_status=3;
    }
    __HAL_GPIO_EXTI_CLEAR_IT(KEY3_GPIO_PIN);
  }
  else if(GPIO_Pin==KEY4_GPIO_PIN)
  {
    HAL_Delay(20);/* ��ʱһС��ʱ�䣬�������� */
    if(HAL_GPIO_ReadPin(KEY4_GPIO,KEY4_GPIO_PIN)==KEY4_DOWN_LEVEL)
    {
      flag_nfc_status=4;
    }
    __HAL_GPIO_EXTI_CLEAR_IT(KEY4_GPIO_PIN);
  }
  else if(GPIO_Pin==KEY5_GPIO_PIN)
  {
    HAL_Delay(20);/* ��ʱһС��ʱ�䣬�������� */
    if(HAL_GPIO_ReadPin(KEY5_GPIO,KEY5_GPIO_PIN)==KEY5_DOWN_LEVEL)
    {
      flag_nfc_status=5;
    }
    __HAL_GPIO_EXTI_CLEAR_IT(KEY5_GPIO_PIN);
  }  
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
