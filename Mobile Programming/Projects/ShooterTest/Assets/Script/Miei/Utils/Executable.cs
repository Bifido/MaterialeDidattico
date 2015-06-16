using UnityEngine;

public interface Executable{
	void Execute();
	void StopExecute();
	bool IsExecuting();

	void SetExcutioner(Executioner exec,int ID);
	void NotifyExecutioner(bool success);
	int GetID();

	void 		SetAllocator(Allocator allocator);
	Allocator 	GetAllocator();
	GameObject 	GetGameObject();
}
