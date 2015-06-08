using UnityEngine;
using System.Collections;

public class PerformanceTest : MonoBehaviour 
{

	void Start()
	{
		if(m_oReferenceQuad != null)
		{
			m_aoQuads = new GameObject[m_iMeshCount];
			for(int i = 0; i < m_iMeshCount; ++i)
			{
				m_aoQuads[i] = GameObject.Instantiate((Object)m_oReferenceQuad, GetRandomPosition(), m_oReferenceQuad.transform.rotation) as GameObject;
				int iTextureOffsetX = Random.Range(0, 9);
				int iTextureOffsetY = Random.Range(0, 9);
				m_aoQuads[i].GetComponent<Renderer>().sharedMaterial.mainTextureOffset = new Vector2(0.1f * iTextureOffsetX, 0.1f * iTextureOffsetY);
			}
		}
	}

	private Vector3 GetRandomPosition()
	{
		Vector3 vPosition = Vector3.zero;
		vPosition.x = Random.Range(-mk_fXSize, mk_fXSize);
		vPosition.y = Random.Range(-mk_fySize, mk_fySize);

		return vPosition;
	}

	void Update()
	{
	
	}

	//VARS
	[SerializeField] private int m_iMeshCount = 30;
	[SerializeField] private GameObject m_oReferenceQuad;

	private const float mk_fXSize = 14.0f;
	private const float mk_fySize = 8.0f;

	private GameObject[] m_aoQuads;
}
