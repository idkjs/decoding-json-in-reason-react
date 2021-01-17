/* this type returns an array */
type state = {repoData: option(array(RepoData.repo))};

/* define action type */
type action =
  | Loaded(array(RepoData.repo));

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
    React.array(
      Array.map(
        (repo: RepoData.repo) => <RepoItem key={repo.full_name} repo />,
        repos,
      ),
    )
  | None => React.string("Loading")
  };

let reducer = (_state, action) =>
  switch (action) {
  | Loaded(loadedRepo) => {repoData: Some(loadedRepo)}
  };

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, {repoData: None});
  let handleReposLoaded = repoData => dispatch(Loaded(repoData));
Js.log("APP")
  React.useEffect0(() => {
    RepoData.fetchRepos()
    |> Js.Promise.then_(repoData => {
         handleReposLoaded(repoData)->ignore;
         Js.Promise.resolve();
       })
    |> ignore;
    None;
  });

  <div className="App">
    <h1> {React.string("Decoding JSON in React")} </h1>
    {repoItems(state.repoData)}
  </div>;
};
