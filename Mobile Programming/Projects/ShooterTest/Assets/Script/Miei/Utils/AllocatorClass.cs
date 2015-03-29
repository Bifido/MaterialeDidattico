using UnityEngine;
using System.Collections;

public class AllocatorClass<T>{

	public AllocatorClass(){}
	public AllocatorClass(int capacity){
		this.m_iCapacity = capacity;

		this.m_oPool = new Pool<T>();
		
		T [] temp = new T[this.m_iCapacity];
		for(int i = 0; i<this.m_iCapacity; i++){
			temp[i] = default(T);
			Debug.Log(i);
		}
		this.m_oPool = new Pool<T>(m_iCapacity,temp);
	}

	public T getElement(){
		return this.m_oPool.getElement();
	}

	public void freeElement(T t){
		this.m_oPool.freeElement(t);
	}
	
	[SerializeField] protected int 	m_iCapacity;
	protected Pool<T> 				m_oPool;
}

