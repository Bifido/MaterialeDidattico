using UnityEngine;
using System.Collections;

public class AllocatorGameObject<T> : AllocatorClass<T> where T:Object {

	public AllocatorGameObject(){}
	public AllocatorGameObject(int capacity,GameObject t,GameObject parent){
		base.m_iCapacity = capacity;
		
		base.m_oPool = new Pool<T>();
		
		T [] temp = new T[this.m_iCapacity];
		for(int i = 0; i<this.m_iCapacity; i++){
			temp[i] = GameObject.Instantiate(t) as T;
			Debug.Log(temp[i].name);
		}
		this.m_oPool = new Pool<T>(m_iCapacity,temp);
	}
}
