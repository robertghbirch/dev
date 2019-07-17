usbd_customhid.c

extern char g_ring_buff[256][10];
extern char g_ring_tail ;
extern char g_ring_head ;
static uint8_t  USBD_CUSTOM_HID_DataOut (USBD_HandleTypeDef *pdev,
                                          uint8_t epnum)
{

  USBD_CUSTOM_HID_HandleTypeDef     *hhid = (USBD_CUSTOM_HID_HandleTypeDef*)pdev->pClassData;

  ((USBD_CUSTOM_HID_ItfTypeDef *)pdev->pUserData)->OutEvent(hhid->Report_buf[0],
                                                            hhid->Report_buf[1]);

  USBD_LL_PrepareReceive(pdev, CUSTOM_HID_EPOUT_ADDR , hhid->Report_buf,
                         USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);


  for (int i = 0 ; i < 10 ; i++) {
	  g_ring_buff[g_ring_tail][i] = hhid->Report_buf[i];
  }
  g_ring_tail++ ;

  return USBD_OK;
}



if (g_ring_head != g_ring_tail) {			// head is chasing the tail

		  for( int i = 0; i < 10; i++)
		  {
			  receive_HIDbuffer[i] = g_ring_buff[g_ring_head][i];
		  }


		  g_ring_head++ ;
	  }









