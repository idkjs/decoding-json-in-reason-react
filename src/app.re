/* defining our state type before defining our component */
type state = {repoData: option(RepoData.repo)};

/* define action type */
type action =
  | Loaded(RepoData.repo);

let component = ReasonReact.reducerComponent("App");

/* our dummy data */
let dummyRepo: RepoData.repo = {
  stargazers_count: 27,
  full_name: "jsdf/reason-react-hacker-news",
  html_url: "https://github.com/jsdf/reason-react-hacker-news",
};

/* let loadReposButton =
   <button onClick=(_event => send(Loaded(dummyRepo)))>
   (ReasonReact.string("Load Repos"))
   </button> */
let repoItem = (repoData: option(RepoData.repo)) =>
  switch (repoData) {
  | Some(repo) => <RepoItem repo />
  | None => ReasonReact.string("Click Button To Load")
  };
/* let repoItem = (repoData: option(RepoData.repo)) =>
   switch (repoData) {
   | Some(repo) => <RepoItem repo />
   | None => ReasonReact.string("Loading")
   }; */

/* for button demo */
let reducer = (action, _state) =>
  switch (action) {
  | Loaded(loadedRepo) => ReasonReact.Update({repoData: Some(loadedRepo)})
  };

let make = _children => {
  ...component,
  initialState: () => {repoData: None},
  reducer,
  /* reducer: ((), _) => ReasonReact.NoUpdate, */
  render: ({state: {repoData}, send}) =>
    <div className="App">
      <h1> (ReasonReact.string("Decoding JSON in Reason")) </h1>
      (repoItem(repoData))
      <button onClick=(_event => send(Loaded(dummyRepo)))>
        (ReasonReact.string("Load Repos"))
      </button>
    </div>,
};