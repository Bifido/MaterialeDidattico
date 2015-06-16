using UnityEngine;
using System.Collections;

public class PassiveBackground : MonoBehaviour,PassiveBackgroundGen{

	void Start(){
		this.enabled = false;
	}

	//PASSIVE_BACKGROUND_GEN INTERFACE---------------------------------------------------------------
	public void Init(int capacity,GameObject prefab,float xPos,float yOffset,float yLimit){
		this.m_iCapacity = capacity;
		this.m_oPrefab = prefab;
		this.m_fXPos = xPos;
		this.m_fYOffset = yOffset;
		this.m_fYLimit = yLimit;
		this.m_aElements = new GameObject[m_iCapacity];

		for(int i = 0; i<m_iCapacity; i++){
			GameObject element = Instantiate(m_oPrefab);
			Vector3 pos = new Vector3(m_fXPos,m_fYOffset*i,0f);
			Debug.Log(pos);
			element.transform.position = new Vector3(m_fXPos,m_fYOffset*i,0f);
			this.m_aElements[i] = element;
		}
	}

	public void StartMove(){
		this.enabled = true;
	}

	public void Update(){
		for(int i = 0; i<m_iCapacity; i++){
			if(this.m_aElements[i].transform.position.y <= m_fYLimit){
				this.m_aElements[i].transform.position = new Vector3(m_fXPos,m_fYOffset*(m_iCapacity-1),0f);
			}else{
				this.m_aElements[i].transform.Translate(Vector3.down*Time.deltaTime*Constants.BACKGROUND_SPEED);
			}
		}
	}
	
	public void StopMove(){
		this.enabled = false;
	}
	
	public void ResetMove(){
		for(int i = 0; i<m_iCapacity; i++){
			this.m_aElements[i].transform.position = new Vector3(m_fXPos,m_fYOffset*i,0f);
		}
	}

	private GameObject 		m_oPrefab;
	private GameObject []	m_aElements;
	private int 			m_iCapacity;
	private float			m_fXPos;
	private float			m_fYOffset;
	private float			m_fYLimit;
	//PASSIVE_BACKGROUND_GEN INTERFACE---------------------------------------------------------------
}
