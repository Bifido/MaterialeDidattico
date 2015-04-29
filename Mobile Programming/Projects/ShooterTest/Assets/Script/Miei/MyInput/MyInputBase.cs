using UnityEngine;
using System;
using System.Collections;

public class MyInputBase{

	public virtual void Init(){
	}

	public virtual void InputUpdate(){
	}

	#region SHOOT AND JUMP
	protected void InternalJumpDetected(){
		if(m_actJumpCallback != null){
			m_actJumpCallback();
		}
	}

	protected void InternalShootDetected(Vector3 shootDirection){
		if(m_actShootCallback != null){
			m_actShootCallback(shootDirection);
		}
	}
	
	public void Activate(Action actJumpInput, Action<Vector3> actShootInput){
		m_actJumpCallback = actJumpInput;
		m_actShootCallback = actShootInput;
		Debug.Log("Input activated with:"+actJumpInput.Method.Name+","+actShootInput.Method.Name);
		m_bActive = true;
	}
	#endregion

	#region MULTIPLE TOUCH
	protected void InternalTouchesCountChanged(short sValue){
		if(m_actTouchesCountChangedCallback != null){
			m_actTouchesCountChangedCallback(sValue);
		}
	}

	public void InternalTouchChanged(short ID,Vector3 pos){
		if(m_actTouchChangedCallback != null){
			m_actTouchChangedCallback(ID,pos);
		}
	}

	public void InternalTouchEnd(short ID){
		if(m_actTouchEndCallBack != null){
			m_actTouchEndCallBack(ID);
		}
	}
	public void Activate(Action<short> actTouchesChanged,Action<short,Vector3> actTouchChanged,Action<short> actTouchEnd){
		m_actTouchesCountChangedCallback = actTouchesChanged;
		m_actTouchChangedCallback = actTouchChanged;
		m_actTouchEndCallBack = actTouchEnd;
		m_bActive = true;
		Debug.Log("Input activated with:"+actTouchChanged.ToString()+","+actTouchChanged.ToString()+","+actTouchEnd+ToString());
	}
	#endregion

	public void Deactivate(){
		m_actJumpCallback = null;
		m_actShootCallback = null;
		m_actTouchesCountChangedCallback = null;
		
		m_bActive = false;
	}
	
	//VARS
	private bool m_bActive = false;
	
	protected event Action m_actJumpCallback = null;
	protected event Action<Vector3> m_actShootCallback = null;
	protected event Action<short> m_actTouchesCountChangedCallback = null;
	protected event Action<short> m_actTouchEndCallBack = null;
	protected event Action<short,Vector3> m_actTouchChangedCallback = null;
}
;