using UnityEngine;
using System.Collections;

[RequireComponent(typeof(Renderer))]
public class TextureLoader : MonoBehaviour 
{
	void Start()
	{
		if(Quality.UseHightQuality())
		{
			m_oRender = gameObject.GetComponent<Renderer>();
			if(m_sTexturePath != "")
			{
				Texture oTexture = Resources.Load<Texture>(m_sTexturePath + m_oRender.material.mainTexture.name + Quality.GetQualitySuffix());
				if(oTexture != null)
				{
					m_oRender.material.mainTexture = oTexture;
				}
			}
		}
	}


	//VARS
	[SerializeField] private string m_sTexturePath = "";

	private Renderer m_oRender; 


}
