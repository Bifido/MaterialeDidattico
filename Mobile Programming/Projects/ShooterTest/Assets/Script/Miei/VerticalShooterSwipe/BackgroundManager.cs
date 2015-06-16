using UnityEngine;
using System.Collections;

public class BackgroundManager : MonoBehaviour {

	// Use this for initialization
	void Start () {
		//GetAll PassiveBackground attached
		PassiveBackground [] backgrounds = this.GetComponents<PassiveBackground>();
		m_aPassiveBackgrounds = backgrounds;

		//Init all PassiveBackground
		m_oWalls = m_aPassiveBackgrounds[(int)BACKGROUNDS.WALLS];
		m_oWalls.Init(Constants.BACKGROUND_WALL_NUMBER,
		              WallPrefab,
		              Constants.BACKGROUND_WALL_X_POS,
		              Constants.BACKGROUND_WALL_Y_OFFSET,
		              Constants.BACKGROUND_WALL_Y_LIMIT);

		//start move backgrounds
		StartMovement();
	}

	void Update(){
		if(Input.GetKeyDown(KeyCode.S)){
			StopMovement();
		}
		if(Input.GetKeyDown(KeyCode.A)){
			StartMovement();
		}
	}

	public void StartMovement(){
		for(int i = 0; i < m_aPassiveBackgrounds.Length; i++){
			m_aPassiveBackgrounds[i].StartMove();
		}
	}

	public void StopMovement(){
		for(int i = 0; i < m_aPassiveBackgrounds.Length; i++){
			m_aPassiveBackgrounds[i].StopMove();
		}
	}

	public GameObject WallPrefab;

	private enum BACKGROUNDS		{ WALLS = 0,COUNT};
	private PassiveBackground [] 	m_aPassiveBackgrounds;
	private PassiveBackground 		m_oWalls;
}
