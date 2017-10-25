void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if (m->menuItems() == nullptr)
		return;

	if (m != nullptr)
	{
		if (m->menuItems() != nullptr)
		{
			vector<MenuItem*>::const_iterator it = m->menuItems()->begin();
			for (; it != m->menuItems()->end(); it++)
			{
				if (path == "")
				{
					string newPath = path + (*it)->name();
					cout << newPath << endl;
					listAll(*it, newPath);
				}
				else
				{
					string newPath = path + '/' + (*it)->name();
					cout << newPath << endl;
					listAll(*it, newPath);
				}
			}
		}
	}
}