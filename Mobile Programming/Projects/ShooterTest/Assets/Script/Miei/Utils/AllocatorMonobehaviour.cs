using UnityEngine;
using System.Collections;

public class AllocatorMonobehaviour<T> where T: MonoBehaviour{
	public AllocatorMonobehaviour(){}
	public AllocatorMonobehaviour(int capacity,GameObject prefab,GameObject parent){
		if(prefab.GetComponent<T>() != null){
			this.m_iCapacity = capacity;
			
			this.m_oPool = new Pool<T>();
			
			T [] temp = new T[this.m_iCapacity];
			for(int i = 0; i<this.m_iCapacity; i++){
				GameObject newGameObject = GameObject.Instantiate(prefab) as GameObject;
				newGameObject.transform.parent = parent.transform;
				temp[i] = newGameObject.GetComponent<T>();
			}
			this.m_oPool = new Pool<T>(m_iCapacity,temp);
		}else{
			throw new MissingComponentException();
		}
	}
	
	[SerializeField] protected int 			m_iCapacity;
	protected Pool<T> 				m_oPool;
}
