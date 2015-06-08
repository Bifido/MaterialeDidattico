using UnityEngine;
using System.Collections;

[RequireComponent(typeof(Light))]
public class LightQualityCheck : MonoBehaviour 
{
	void Start()
	{
		if((int)Quality.GetQualityLevel() < (int)m_eMinShadowQualityLevel)
		{
			GetComponent<Light>().shadows = LightShadows.None;
		}
	}

	//VARS
	[SerializeField] private Quality.eQualityLevel m_eMinShadowQualityLevel = Quality.eQualityLevel.BASE;
}
