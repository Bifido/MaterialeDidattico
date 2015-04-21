using UnityEngine;
using System;
using System.Collections;

public class MyInputBase{

	public virtual void InputUpdate(){
	}
	
	protected void InternalJumpDetected(){
		if(m_actJumpCallback != null){
			m_actJumpCallback();
		}
	}
	
//	protected void InternalShootDetected()
//	{
//		if(m_actShootCallback != null)
//		{
//			m_actShootCallback();
//		}
//	}

	protected void InternalShootDetected(Vector3 shootDirection){
		if(m_actShootCallback != null){
			m_actShootCallback(shootDirection);
		}
	}
	
	public void Activate(Action actJumpInput, Action<Vector3> actShootInput){
		m_actJumpCallback = actJumpInput;
		m_actShootCallback = actShootInput;
		
		m_bActive = true;
	}

	protected void InternalTouchesCountChanged(bool boolean){
		if(m_actTouchesCountChangedCallback != null){
			m_actTouchesCountChangedCallback(boolean);
		}
	}

	public void Activate(Action<bool> actTouchesChanged){
		m_actTouchesCountChangedCallback = actTouchesChanged;

		m_bActive = true;
	}
	
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
	protected event Action<bool> m_actTouchesCountChangedCallback = null;
}
