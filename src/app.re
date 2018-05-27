/* this type returns an array */
type state = {repoData: option(array(RepoData.repo))};

/* define action type */
type action =
  | Loaded(array(RepoData.repo));

let component = ReasonReact.reducerComponent("App");

let dummyRepos: array(RepoData.repo) = [|
  RepoData.parseRepoJson(
    Js.Json.parseExn(
      {js|
        {
          "stargazers_count": 93,
          "full_name": "reasonml/reason-tools",
          "html_url": "https://github.com/reasonml/reason-tools"
        }
      |js},
    ),
  ),
|];

let repoItems = (repoData: option(array(RepoData.repo))) =>
  switch (repoData) {
  | Some(repos) =>
    ReasonReact.array(
      Array.map(
        (repo: RepoData.repo) => <RepoItem key=repo.full_name repo />,
        repos,
      ),
    )
  | None => ReasonReact.string("Loading")
  };

let reducer = (action, _state) =>
  switch (action) {
  | Loaded(loadedRepo) => ReasonReact.Update({repoData: Some(loadedRepo)})
  };

let make = _children => {
  ...component,
  initialState: () => {repoData: None},
  didMount: self => {
    let handleReposLoaded = repoData => self.send(Loaded(repoData));

    RepoData.fetchRepos()
    |> Js.Promise.then_(repoData => {
         handleReposLoaded(repoData);
         Js.Promise.resolve();
       })
    |> ignore;
  },
  reducer,
  render: ({state: {repoData}}) =>
    <div className="App">
      <h1> (ReasonReact.string("Decoding JSON in ReasonReact")) </h1>
      (repoItems(repoData))
    </div>,
};