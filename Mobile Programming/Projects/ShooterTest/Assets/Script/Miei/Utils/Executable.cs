using UnityEngine;

public interface Executable{
	void Execute();
	void StopExecute();
	bool IsExecuting();

	void SetExcutioner(Executioner exec);
	void NotifyExecutioner(bool success);

	void 		SetAllocator(Allocator allocator);
	Allocator 	GetAllocator();
	GameObject 	GetGameObject();
}
