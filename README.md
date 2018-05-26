# Decoding API Response JSON in ReasonReact

Reason is a new statically-typed functional programming language from Facebook which can be compiled to Javascript. Reason React is a wrapper for React which makes it easy to use from Reason.

We're going to build a small single page web app to put Reason React through its paces. The app will display a list of top Reason-related Github repos. It's a small enough task that we can complete it in a few hours, but also has enough complexity that we can kick the tires of this new language. This tutorial expects no existing knowledge of Reason, though a basic familiarity with static types would be helpful.

# A New roject

We're going to use `create-react-app` using `reason-scripts` which will create a starting point for our app, which is going to be called `decoding-reason`:

```
yarn create react-app decoding-reason -- --scripts-version reason-scripts
```

Start the project by running `yarn start` which should open `https://localhost:3000` in your browser.

# A Record Type

We will be decoding a response from the public data from this Github API request: https://api.github.com/search/repositories?q=topic%3Areasonml&type=Repositories but first lets set it up using fake data.

Create a new file called RepoData.re and add this code into it.

```
type repo = {
  full_name: string,
  stargazers_count: int,
  html_url: string
};
```

# Files are modules

We've defined our type at the top level of the file.

> Top-level basically means visually, no indentation (very poor way of explaining it, but you get the idea). a let foo = 1 that’s not nested inside anything else is top-level. a let foo = 1 that’s inside a function body isn’t at the top-level. [@chenglou](https://reasonml.chat/t/bucklescript-top-level-declarations/399/3?u=idkjs)

In Reason, every file is a module, and all the things defined at the top level of the file using the keywords let, type, and module are exposed to be used from other files (that is, other modules). In this case, other modules can reference our repo type as RepoData.repo. Unlike in Javascript, no imports are required to reference things from other modules.

Let's use our type in app.re. The repos page is just a list of repos, with each item in the list consisting of the name of the repo (linking to the repo on Github), and the number of stars the repo has. We'll define some dummy data and sketch out a new component called `RepoItem` in `RepoItem.re` to represent an item in the list of repos:

```
//RepoItem.re
let component = ReasonReact.statelessComponent("RepoItem");

let make = (_children) => {
  ...component,
  render: (_self) => {
    /* our dummy data */
    let dummyRepo: RepoData.repo = {
      stargazers_count: 27,
      full_name: "jsdf/reason-react-hacker-news",
      html_url: "https://github.com/jsdf/reason-react-hacker-news"
    };

    <div className="App">
      <h1>{ReasonReact.stringToElement("Reason Projects")}</h1>
      <RepoItem repo=dummyRepo />
    </div>
  }
};
```

In the statement beginning `let dummyRepo: RepoData.repo = {...}`, `dummyRepo` is the name of the constant we're defining and `RepoData.repo` is the type we're annotating it with which is coming from where we defined it in `RepoData.re`. Remember that this module is available anywhere in the project. Reason can infer the types of most things we declare, but here it's useful to include the annotation so that the typechecker can let us know if we've made a mistake in our test data.

# Return values in Reason

Note that the body of the render function is now wrapped in {} braces, because it contains multiple statements. In Javascript, if we used braces around the body of an => arrow function we'd need to add a return statement to return a value. However in Reason, the value resulting from the last statement in the function, here:

```jsx
...
<div className="App">
      <h1>{ReasonReact.stringToElement("Reason Projects")}</h1>
      <RepoItem repo=dummyRepo />
</div>
```

automatically becomes the return value. If you don't want to return anything from a function, you can make the last statement `()` (which is called 'unit' in [Reason](https://reasonml.github.io/docs/en/function.html#optional-labeled-arguments)).

# Defining components in Reason React

You might now see an error saying The module or file `RepoItem`can't be found. That's because we added `<RepoItem repo=dummyRepo />` in the render function of the `App component`, but we haven't created that module yet. Add a new file called `RepoItem.re` containing:

```
let component = ReasonReact.statelessComponent("RepoItem");

let make = (~repo: RepoData.repo, _children) =>
  {
    ...component,
    render: (_self) =>
      <div>{ReasonReact.string(repo.full_name)}</div>
  };
```

What's going on here? Let's dissect what's happening in this file.

Each Reason React component is a Reason module which defines a function called make, which defines props and children arguments. The props are specified as [Labeled Arguments](https://reasonml.github.io/docs/en/function.html#labeled-arguments).

```
let component = ReasonReact.statelessComponent("SomeComponent");

let make = (~someProp, ~anotherProp, _children) => /* some stuff here */;
```

This `make` function returns a record. The first thing in this record is typically `...component`, where `component` is the return value of `ReasonReact.reducerComponent` or `ReasonReact.statelessComponent` (for components which do and don't use state, respectively). If this seems a bit weird, just think of it as inheriting from the React component class, like the equivalent of doing `class Foo extends React.Component {...` in ReactJS.

```
//RepoItem.re
let component = ReasonReact.statelessComponent("RepoItem");

let make = (~someProp, ~anotherProp, _children) =>
  {
    ...component,
    /* render and lifecycle methods go here */
  };
```

The rest of the record is where you can add the `render` function and the [lifecycle methods](https://reasonml.github.io/reason-react/docs/en/lifecycles.html#content) you're used to from React.

So back to RepoItem.re:

```
let component = ReasonReact.statelessComponent("RepoItem");

let make = (~repo: RepoData.repo, _children) =>
  {
    ...component,
    render: (_self) =>
      <div>{ReasonReact.stringToElement(repo.full_name)}</div>
  };
```

What we have here is a `stateless component` which takes one prop called `repo`, (annotated with the `type repo` from the `RepoData` module), and renders a `div`.

In ReactJS, you can use `this.props` to access the component's props inside the `render` method. In ReasonReact we instead receive the props as labeled arguments to the `make` function, and we can use them inside our `render` function directly (as we are doing to access the `full_name` field of the `repo` prop above).

The `make` function also get passed a `children` argument, but we aren't making use of `children` in this component so we put an `_` underscore at the start of the `_children` argument name. That just lets Reason know we're not actually using that argument. Despite the fact we're not using this argument, it does still need to be included in the function arguments or you'll get an error.

Next we'll flesh out the `render` function to present the fields of the `repo` record:

```
let component = ReasonReact.statelessComponent("RepoItem");

let make = (~repo: RepoData.repo, _children) =>
  {
    ...component,
    render: (_self) =>
      <div className="RepoItem">
        <a href=repo.html_url>
          <h2>{ReasonReact.string(repo.full_name)}</h2>
        </a>
        {ReasonReact.string(string_of_int(repo.stargazers_count) ++ " stars")}
      </div>
  };
```

Note that we have to convert the `int` value of `repo.stargazers_count` to a `string` using the [`string_of_int`](https://reasonml.github.io/docs/en/faq.html#where-do-all-these-print-endline-string-of-int-functions-come-from) function. We then use the `++` string concatenation operator to combine it with the string " stars".

Now is a good time to save and take a look at our progress in the browser.

![screenshot](./screenshot1.png)

# A Stateful React Component aka `reducerComponent` and `Variants`

Our app is going to load some data and then render it, which means we need a place to put the data after it's loaded. React component state seems like an obvious choice. So we'll make our App component stateful. We do that by changing our `ReasonReact.statelessComponent` to a [`ReasonReact.reducerComponent`](https://reasonml.github.io/reason-react/docs/en/state-actions-reducer.html#docsNav).

In `App.re`:

```
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
```

We've changed some key things: we've defined a `type` for the `state` of our `component` which uses Reason's built in `option` `Variant` type. Here simply called `state`, `ReasonReact.statelessComponent` has become `ReasonReact.reducerComponent`, we've added an `initialState` function to the `component`, and we've changed `render` to take `self` as it's argument (removing the leading `_` underscore so that self is no longer ignored), which is now being used to pass `{state:{repoData}` as a prop to `RepoItem`. What?!! The syntax below is called `destructing`. The `self` method we are now accessing has a `state` property on it and by using `{state:...}` we are saying take the the `state` from `self` and use it in the following function.

`render: ({state: {repoData}}) =>`

## [Variant! Options](https://reasonml.github.io/docs/en/variant.html#docsNav)

We defined our `state` type as:

```
type state = {repoData: option(RepoData.repo)};
```

In Reason `option` is a `type` which is made up of 'Variants'. That basically means that a value of this `type` can be one of several possible variations which have been explicitly defined. In the case of `option`, the variants are `Some` and `None`. `Some` is used when a value is present (and contains the value itself), whereas None represents the absence of a value (like null in Javascript). Here we've 'wrapped' dummyRepo in the `Some` variant, because we have a value present.

The `option` tells us (and the compiler) that the `state` can be either of `Some` value or `None` for no value. So there will be `Some(RepoData.repo)` or `None` when we use this `type`.

So why use this wrapper, instead of just allowing our repoData field to contain either a value or null? The reason is to force us to handle both possible cases when actually using the value. This is good because it means we can't accidentally forget to deal with the 'null' case.
Note that the `state` type must be defined before the call to `ReasonReact.reducerComponent` or you'll get an error saying something like "The type constructor state would escape its scope". We will use this to tell our component what to do in each case by creating a variable called `repoItem` and defining what we want to happen for the variant cases defined in `type state`.

# Option and Pattern Matching

Currently we have our `repoData` dummy data already available when we define the initial state of the component, but once we are loading it from the server it will initially be `null`. However, in Reason you can't just have the value of a record field be `null`, as you can in Javascript. Instead, things which might not be present need to be 'wrapped' in another type called `option`. We can change our state type to represent this like so:

`type state = {repoData: option(RepoData.repo)};`

and in our initialState function we wrap our repo record in `Some()`:

```
  initialState: () => {
    repoData: Some(dummyRepo),
  },
```

In the code above, we are using `Some` and `None` Variants to define a `repoItem` where if there is `Some` data, we pass that data to our `<RepoItem />` module and return it for rendering to the ui in our component. If there is no data, the we tell the function to use the `None` option, returning a `div` which renders "Loading" to the ui.

Then in our rendering `div` we are passing the current `repoData` which we then pass to the `renderItem` function to deal with what to do in each case, `Some` or `None`.

**We can't pass `state.repoData` directly as the repo prop of RepoItem, because it's wrapped in an option()**, but `RepoItem` expects it **without** the `option` wrapper. So how do we unwrap it? We use **pattern matching**. This is where Reason forces use to cover all possible cases (or at least explicitly throw an error). Pattern matching makes use of the `switch` statement. Unlike a switch statement in Javascript however, the cases of a switch statement in Reason can match the types of the values (eg. `Some` and `None`), not just the values themselves. We'll change our render method to use a `switch` to provide logic to render our repo item in each possible case. We can do that by creating a function, `renderItem` that handles each case and renders based on the result.

```
let repoItem = (repoData: option(RepoData.repo)) =>
  switch (repoData) {
  | Some(repo) => <RepoItem repo />
  | None => ReasonReact.string("Loading")
  };
```

Here you can see the `switch` statement has a case to match a `state.repoData` value with the type Some, and pulls out the actual repo record into a variable called `repo`, which it then uses in the expression to the right of the `=>`, returning a `<RepoItem>` element. This expression will only be used in the case where `state.repoData` is `Some`. Alternatively, if `state.repoData` is `None`, the text "Loading" will be displayed instead.

We will call `repoItem` in our `div` and pass it `state.repoData` which we destructured as `repoData`.

```
  render: ({state: {repoData}}) =>
    <div className="App">
      <h1> (ReasonReact.string("Decoding JSON in Reason")) </h1>
      (repoItem(repoData))
    </div>,
```

If you run `yarn start` you should see the same output as before in the browswer:

![screenshot](./screenshot2.png)
