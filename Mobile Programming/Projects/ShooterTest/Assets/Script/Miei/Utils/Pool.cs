using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class Pool{

	public Pool(int capacity,Executioner container,Executable contained){
		this.m_oElements = new ArrayList(capacity);
		this.m_iNextFreeElement = new LinkedList<int>();
		for(int i = 0;i<this.m_oElements.Capacity;i++){
			GameObject element = GameObject.Instantiate(contained) as GameObject;
			this.m_oElements.Add(element);
			this.m_iNextFreeElement.AddLast(i);
		}
	}

	public GameObject getElement(){
		if(this.m_iNextFreeElement.Count > 0){
			int indexNextFree = m_iNextFreeElement.First.Value;
			m_iNextFreeElement.RemoveFirst();
			return this.m_oElements[indexNextFree] as GameObject;
		}else{
			return null;
		}
	}

	public void returnElement(GameObject element){
		this.m_iNextFreeElement.AddLast(this.m_oElements.IndexOf(element));
	}

	private ArrayList m_oElements;
	private LinkedList<int> m_iNextFreeElement;
}
