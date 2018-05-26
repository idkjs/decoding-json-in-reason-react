/* defining our state type before defining our component */
type state = {repoData: option(RepoData.repo)};

let component = ReasonReact.reducerComponent("App");

/* our dummy data */
let dummyRepo: RepoData.repo = {
  stargazers_count: 27,
  full_name: "jsdf/reason-react-hacker-news",
  html_url: "https://github.com/jsdf/reason-react-hacker-news",
};

let repoItem = (repoData: option(RepoData.repo)) =>
  switch (repoData) {
  | Some(repo) => <RepoItem repo />
  | None => ReasonReact.string("Loading")
  };

let make = _children => {
  ...component,
  initialState: () => {repoData: Some(dummyRepo)},
  reducer: ((), _) => ReasonReact.NoUpdate,
  render: ({state: {repoData}}) =>
    <div className="App">
      <h1> (ReasonReact.string("Decoding JSON in Reason")) </h1>
      (repoItem(repoData))
    </div>,
};