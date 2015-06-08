using UnityEngine;
using System.Collections;

public class LightCycle : MonoBehaviour {

	// Use this for initialization
	void Start () {
		m_DaySect = (int) DAY_SECTION.DAWN;
	}
	
	// Update is called once per frame
	void Update () {
		if(Input.GetKeyDown(KeyCode.L)){
			ChangeDayTime();
		}
	}

	public void ChangeDayTime(){
		m_DaySect = ++m_DaySect % (int)DAY_SECTION.COUNT;
		switch(m_DaySect){
		case(0):
			Dawn();
		break;

		case(1):
			Mid();
		break;

		case(2):
			Night();
		break;

		default:
		break;
		}
	}

	private void Dawn(){
		this.m_Anim.SetBool("Night",false);
		this.m_Anim.SetBool("Dawn",true);
	}
	private void Mid(){
		this.m_Anim.SetBool("Dawn",false);
		this.m_Anim.SetBool("Mid",true);
	}
	private void Night(){
		this.m_Anim.SetBool("Mid",false);
		this.m_Anim.SetBool("Night",true);
	}

	public Animator m_Anim;
	public enum DAY_SECTION { DAWN = 0, MID = 1, NIGHT = 2, COUNT = 3};

	private int m_DaySect;
}
