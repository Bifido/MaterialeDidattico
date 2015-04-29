using UnityEngine;
using System.Collections;

public class MyInputManager : MonoBehaviour {

	public delegate void InputEventEmpty();
	public event InputEventEmpty OnJump;
	public delegate void InputEvent(Vector3 endDir);
	public event InputEvent OnShoot;
	public delegate void InputEventShort(short iValue);
	public event InputEventShort OnNewTouch;
	public event InputEventShort OnEndTouch;
	public delegate void InputEventShortAndVector(short iValue,Vector3 pos);
	public event InputEventShortAndVector OnTouchChangedEvent;
	
	void Start(){
		InitInput();
	}
	
	void Update(){
		if(m_oInput != null){
			m_oInput.InputUpdate();
		}
	}
	
	private void JumpDetected(){
		if(OnJump != null){
			OnJump();
		}
	}
	
	private void ShootDetected(Vector3 endDiretction){
		if(OnShoot != null){
			OnShoot(endDiretction);
		}
	}

	private void OnTouchCountChanged(short sValue){
		if(OnNewTouch != null){
			OnNewTouch(sValue);
		}
	}

	private void OnTouchChanged(short ID, Vector3 pos){
		if(OnTouchChangedEvent != null){
			OnTouchChangedEvent(ID,pos);
		}
	}

	private void OnTouchEnd(short ID){
		if(OnEndTouch != null){
			OnEndTouch(ID);
		}
	}
	
	private void InitInput(){
		m_oInput = MyInputFactory.GetInput(m_eInputSource);
		
		if(m_oInput != null){
			if(m_eInputSource == eMyInputSource.TOUCH_COUNTER){
				m_oInput.Activate(OnTouchCountChanged,OnTouchChanged,OnTouchEnd);
			}else{
				m_oInput.Activate(JumpDetected, ShootDetected);
			}
		}
	}
	
	public void ChangeInput(eMyInputSource eNewInputSource){
		if(eNewInputSource != m_eInputSource){
			if(m_oInput != null){
				m_oInput.Deactivate();
				m_oInput = null;
			}
			
			m_eInputSource = eNewInputSource;
			
			InitInput(); 
		}
	}
	
	//VARS
	public enum eMyInputSource{
		PLAYER = 0,
		TOUCH_COUNTER,
		AI,
		REPLAY,
		NETWORK,
		COUNT
	}
	
	[SerializeField] private eMyInputSource m_eInputSource;
	
	private MyInputBase m_oInput;
}
