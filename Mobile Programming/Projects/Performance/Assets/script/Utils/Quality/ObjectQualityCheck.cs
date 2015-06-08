using UnityEngine;
using System.Collections;

public class ObjectQualityCheck : MonoBehaviour 
{

	void Start()
	{
		if((int)Quality.GetQualityLevel() < (int)m_eMinQualityLevel)
		{
			this.gameObject.SetActive(false);
		}
	}

	//VARS
	[SerializeField] private Quality.eQualityLevel m_eMinQualityLevel = Quality.eQualityLevel.BASE;
}
