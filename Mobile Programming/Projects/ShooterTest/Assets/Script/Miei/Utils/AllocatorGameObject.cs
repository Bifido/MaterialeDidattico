using UnityEngine;
using System.Collections;

public class AllocatorGameObject{

	public AllocatorGameObject(){}
	public AllocatorGameObject(int capacity,GameObject prefab,GameObject parent){
		this.m_iCapacity = capacity;
		
		this.m_oPool = new Pool<GameObject>();
		
		GameObject [] temp = new GameObject[this.m_iCapacity];
		for(int i = 0; i<this.m_iCapacity; i++){
			GameObject newGameObject = GameObject.Instantiate(prefab) as GameObject;
			newGameObject.transform.parent = parent.transform;
			temp[i] = newGameObject;
		}
		this.m_oPool = new Pool<GameObject>(m_iCapacity,temp);
	}

	[SerializeField] protected int 			m_iCapacity;
	protected Pool<GameObject> 				m_oPool;
}
